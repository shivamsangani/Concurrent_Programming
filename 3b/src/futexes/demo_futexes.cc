#include <chrono>
#include <cstddef>
#include <iostream>
#include <random>
#include <string>
#include <thread>
#include <vector>

#include "src/futexes/mutex_hybrid.h"
#include "src/futexes/mutex_semaphore.h"
#include "src/futexes/mutex_simple.h"
#include "src/futexes/mutex_smart.h"
#include "src/futexes/mutex_spinning.h"

template <typename MutexType>
class MutexManager {
 public:
  explicit MutexManager(MutexType& mutex) : mutex_(mutex) { mutex.Lock(); }

  ~MutexManager() { mutex_.Unlock(); }

 private:
  MutexType& mutex_;
};

template <typename MutexType>
void ThreadBody(MutexType& mutex, size_t& counter, size_t increments_per_thread,
                bool waste_time) {
  for (size_t iteration = 0; iteration < increments_per_thread; iteration++) {
    mutex.Lock();
    counter++;
    if (waste_time) {
      size_t value = ((iteration % 10) + 10) * 100;
      for (volatile size_t i = 0; i < value; i = i + 1) {
        // do nothing
      }
    }
    mutex.Unlock();
  }
}

template <typename MutexType>
void RunBenchmark(size_t num_threads, size_t increments_per_thread,
                  bool waste_time) {
  std::cout << "Running " << MutexType::GetName() << " benchmark" << std::endl;
  std::cout << "  num_threads:           " << num_threads << std::endl;
  std::cout << "  increments_per_thread: " << increments_per_thread
            << std::endl;
  std::cout << "  waste_time:            " << waste_time << std::endl;

  auto begin_time = std::chrono::high_resolution_clock::now();

  MutexType mutex;
  size_t counter = 0;
  std::vector<std::thread> threads;
  for (size_t i = 0; i < num_threads; i++) {
    threads.emplace_back(ThreadBody<MutexType>, std::ref(mutex),
                                  std::ref(counter), increments_per_thread,
                                  waste_time);
  }
  for (auto& thread : threads) {
    thread.join();
  }

  auto end_time = std::chrono::high_resolution_clock::now();

  size_t expected_value = num_threads * increments_per_thread;
  if (counter != expected_value) {
    std::cerr << "Error: total number of increments is " << counter
              << ", expected " << expected_value << std::endl;
    exit(1);
  }

  auto duration = end_time - begin_time;
  auto millis =
      std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
  std::cout << "Duration: " << millis << " ms" << std::endl;
}

int main(int argc, char** argv) {
  if (argc != 5) {
    std::cerr << "Usage: " << argv[0]
              << R"( mutex_kind num_threads increments_per_thread waste_time
  mutex_kind values:
     semaphore
     spinning
     simple
     smart
     hybrid
)";
    return 1;
  }
  std::string mutex_kind(argv[1]);
  size_t num_threads = std::stoul(std::string(argv[2]));
  size_t increments_per_thread = std::stoul(std::string(argv[3]));
  bool waste_time = std::stoul(std::string(argv[4]));

  if (mutex_kind == "semaphore") {
    RunBenchmark<MutexSemaphore>(num_threads, increments_per_thread,
                                 waste_time);
  } else if (mutex_kind == "spinning") {
    RunBenchmark<MutexSpinning>(num_threads, increments_per_thread, waste_time);
  } else if (mutex_kind == "simple") {
    RunBenchmark<MutexSimple>(num_threads, increments_per_thread, waste_time);
  } else if (mutex_kind == "smart") {
    RunBenchmark<MutexSmart>(num_threads, increments_per_thread, waste_time);
  } else if (mutex_kind == "hybrid") {
    RunBenchmark<MutexHybrid>(num_threads, increments_per_thread, waste_time);
  } else {
    std::cerr << "Unknown mutex kind: " << mutex_kind << std::endl;
    return 1;
  }
  return 0;
}
