#ifndef SPIN_LOCK_ACTIVE_BACKOFF_H
#define SPIN_LOCK_ACTIVE_BACKOFF_H

#include <atomic>
#include <cstddef>
#include <string>

class SpinLockActiveBackoff {
 public:
  SpinLockActiveBackoff() : held_(false){}

  void Lock() {
    while(held_.exchange(true) == true)
    {
      //failed to acquire lock
      while(held_.load()) { 
        for(volatile size_t i = 0; i < 100; i = i + 1) {}
      }
      //when held bit is false, we exit loop and might be able to acquire the lock 
    }
    //we got lock
  }

  void Unlock() {
    held_.store(false); 
  }
  [[nodiscard]] static std::string GetName() { return "Active backoff"; }

 private:
  // TODO
  std::atomic<bool> held_; 
};

#endif  // SPIN_LOCK_ACTIVE_BACKOFF_H
