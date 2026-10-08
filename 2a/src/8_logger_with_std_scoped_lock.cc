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
    /* Scoped locks are so useful, that the `mutex` header file provides one! */
    std::scoped_lock<std::mutex> doesnt_matter_what_we_call_it(mutex_);
    if (contents.size() + contents_.size() > max_capacity_) {
      return;
    }
    contents_ += contents;
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
