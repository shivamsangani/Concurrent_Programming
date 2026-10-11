#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

template <typename T>
class LockedQueue {
 public:
  explicit LockedQueue(size_t capacity) : head_(0), tail_(0), size_(0) {
    contents_.resize(capacity);
  }

  void Enq(T item) {
    // Must use a lock that supports locking and re-locking.
    std::unique_lock<std::mutex> lock(mutex_);
    // Wait until the condition in the lambda becomes true,
    // going to sleep if it is found to be false.
    not_full_.wait(lock, [&]() -> bool {
      return size_ < contents_.size();
    });
    contents_[head_] = item;
    head_ = (head_ + 1) % contents_.size();
    size_++;
    // Notify anyone waiting to dequeue that there is now an element
    // to be dequeued.
    not_empty_.notify_one();
  }

  T Deq() {
    std::unique_lock<std::mutex> lock(mutex_);
    // Wait until there is an element to be dequeued.
    not_empty_.wait(lock, [&]() -> bool {
      return size_ > 0;
    });
    T result = contents_[tail_];
    tail_ = (tail_ + 1) % contents_.size();
    size_--;
    // Notify any threads waiting to enqueue that there is now
    // space for enqueuing.
    not_full_.notify_one();
    return result;
  }

 private:
  size_t head_;
  size_t tail_;
  size_t size_;
  std::vector<T> contents_;
  std::mutex mutex_;
  std::condition_variable not_full_;
  std::condition_variable not_empty_;
};

int main() {
  const size_t NUM_CONSUMERS = 8;
  const size_t NUM_ELEMS_TO_PRODUCE = 1 << 24;
  const size_t NUM_ELEM_PER_CONSUMER = NUM_ELEMS_TO_PRODUCE / NUM_CONSUMERS;

  LockedQueue<int> producer_to_consumers(256);
  LockedQueue<int> consumers_to_producer(NUM_CONSUMERS);

  int final_total = 0;

  std::thread producer(
      [&producer_to_consumers, &consumers_to_producer, &final_total]() -> void {
        for (size_t i = 0; i < NUM_ELEMS_TO_PRODUCE; i++) {
          producer_to_consumers.Enq(1);
        }

        for (size_t i = 0; i < NUM_CONSUMERS; i++) {
          final_total += consumers_to_producer.Deq();
        }
      });

  std::vector<std::thread> consumers;
  for (size_t i = 0; i < NUM_CONSUMERS; i++) {
    consumers.emplace_back([&producer_to_consumers, &consumers_to_producer]() -> void {
      int partial_sum = 0;
      for (size_t j = 0; j < NUM_ELEM_PER_CONSUMER; j++) {
        partial_sum += producer_to_consumers.Deq();
      }

      consumers_to_producer.Enq(partial_sum);
    });
  }

  producer.join();
  for (auto& consumer : consumers) {
    consumer.join();
  }

  std::cout << "final total: " << final_total << std::endl;

  return 0;
}
