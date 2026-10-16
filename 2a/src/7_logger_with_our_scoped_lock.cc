#include <cstddef>
#include <thread> 
#include <iostream> 
#include <vector>
#include <functional>
#include <mutex> 
#include <sstream>

// class ScopedLock { 
// public: 
//   ScopedLock(std::mutex& mutex) : mutex_(mutex) { 
//     mutex_.lock(); 
//   }

//   ~ScopedLock() { 
//     mutex_.unlock(); 
//   }

// private: 
//   std::mutex& mutex_; 
// };

class Logger { 
public: 
  explicit Logger(size_t max_capacity) : max_capacity_(max_capacity) {}

  void Log(const std::string contents) { 
    std::scoped_lock<std::mutex> lock(mutex_);  
    
    //any threads that get here must hold the mutex
    if(contents.size() + contents_.size() > max_capacity_)
    {
      return; 
      //destruct scoped lock - release the mutex 
    }
    contents_ += contents;
    //destruct scoped lock - release the mutex 
  }

  //method does not modify any members of the class 
  std::string GetContents() const { 
    return contents_; 
  }

private:
  std::string contents_; 
  std::mutex mutex_; 
  size_t max_capacity_; 
};

static void ThreadBody(size_t id, std::vector<std::string>& favourite_foods, Logger& logger) {
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
    "sticky toffee pudding", 
    "chicken", 
    "toad in the hole", 
    "bread pudding"
  };
  Logger logger(500); 

  for(size_t i = 0; i < 8; i++)
  {
    threads.push_back(std::thread(ThreadBody, i, std::ref(favourite_foods), std::ref(logger)));
  }

  for(auto& thread: threads)
  {
    thread.join(); 
  }

  std::cout << logger.GetContents(); 

  return 0; 

 }

