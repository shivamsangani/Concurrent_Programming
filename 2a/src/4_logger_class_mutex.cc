#include <functional>
#include <iostream>
#include <mutex>
#include <sstream>
#include <thread>
#include <vector>

class Logger {
 public:
  void Log(const std::string contents) {
    /* Use a mutex to provide exclusive access to the body of Log.
     * This avoids a data race on contents_. */
    mutex_.lock();
    contents_ += contents;
    mutex_.unlock();
  }

  std::string GetContents() const { return contents_; }

 private:
  std::string contents_;
  std::mutex mutex_;
};

static void ThreadBody(size_t id, std::vector<std::string>& favourite_foods,
                       Logger& logger) {
  /* A stringstream is similar to a StringBuilder in Java/Kotlin */
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
  Logger logger;

  for (size_t i = 0; i < 8; i++) {
    threads.push_back(std::thread(ThreadBody, i, std::ref(favourite_foods), std::ref(logger)));
  }

  for (auto& thread : threads) {
    thread.join();
  }

  std::cout << logger.GetContents();

  return 0;
}
