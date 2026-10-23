#ifndef SPIN_LOCK_PASSIVE_BACKOFF_H
#define SPIN_LOCK_PASSIVE_BACKOFF_H

#include <cstddef>
#include <emmintrin.h>

#include <atomic>
#include <string>

class SpinLockPassiveBackoff {
 public:
  SpinLockPassiveBackoff() : held_(false) {}

  void Lock() {
    while(held_.exchange(true) == true)
    {
      //failed to acquire lock
      while(held_.load()) { 
        for(size_t i = 0; i < 4; i++)
        {
          _mm_pause(); 
        }
      }
      //when held bit is false, we exit loop and might be able to acquire the lock 
    }
    //we got lock
  }

  void Unlock() {
    held_.store(false); 
  }

  [[nodiscard]] static std::string GetName() { return "Passive backoff"; }

 private:
  std::atomic<bool> held_; 
};

#endif  // SPIN_LOCK_PASSIVE_BACKOFF_H
