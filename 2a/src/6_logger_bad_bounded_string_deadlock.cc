#include <algorithm>
#include <functional>
#include <iostream>
#include <mutex>
#include <sstream>
#include <thread>
#include <vector>

class Logger {
 public:
  explicit Logger(size_t max_capacity) : max_capacity_(max_capacity) {}

  void Log(const std::string contents) {
    mutex_.lock();
    /* By moving the early return in inside the mutex, there is no longer a data race
    *  on contents, avoiding concurrent calls to `Log` from overflowing our Log's capacity.
    *  But this solution has another problem: The first thread that overflows the log will
    *  return withour releasing `mutex_`. Any other threads waiting on `mutex_` will be stuck
    *  waiting forever, resulting in a deadlock. */
    if (contents.size() + contents_.size() > max_capacity_) {
      return;
    }
    contents_ += contents;
    mutex_.unlock();
  }

  std::string GetContents() const { return contents_; }

 private:
  std::string contents_;
  std::mutex mutex_;
  size_t max_capacity_;
};

static void ThreadBody(size_t id, std::vector<std::string>& favourite_foods,
                       Logger& logger) {
  std::stringstream ss;
  ss << "Hello, my name is " << std::this_thread::get_id()
     << " but you can call me " << id << " and my favourite food is "
     << favourite_foods[id] << "\n";
  logger.Log(ss.str());
}

int main() {
  std::vector<std::thread> threads;
  std::vector<std::string> favourite_foods = {
      "pizza",
      "steak",
      "broccoli",
      "lobster",
      "Sticky toffee pudding",
      "chicken",
      "toad in the hole",
      "bread pudding",
  };
  Logger logger(200);

  for (size_t i = 0; i < 8; i++) {
    threads.push_back(std::thread(ThreadBody, i, std::ref(favourite_foods), std::ref(logger)));
  }

  for (auto& thread : threads) {
    thread.join();
  }

  std::cout << logger.GetContents();

  return 0;
}
