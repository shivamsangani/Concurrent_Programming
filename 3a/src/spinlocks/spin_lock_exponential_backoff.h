#ifndef SPIN_LOCK_EXPONENTIAL_BACKOFF_H
#define SPIN_LOCK_EXPONENTIAL_BACKOFF_H

#include <cstddef>
#include <emmintrin.h>

#include <atomic>
#include <string> 

class SpinLockExponentialBackoff {
 public:
  SpinLockExponentialBackoff() : held_(false){}

  void Lock() {
    const size_t kMinBackOffIterations = 8; 
    const size_t kMaxBackOffIterations = 1 << 10; 

    size_t backoffIterations = kMinBackOffIterations; 

    while(held_.exchange(true) == true)
    {
      do{
        for(size_t i = 0; i < backoffIterations; i++) {
          _mm_pause(); 
        }
        backoffIterations = std::min(kMaxBackOffIterations, backoffIterations*2); 
      } while (held_.load()); 
    }
  }

  void Unlock() {
    held_.store(false); 
  }

  [[nodiscard]] static std::string GetName() { return "Exponential backoff"; }

 private:
  std::atomic<bool> held_; 
};

#endif  // SPIN_LOCK_EXPONENTIAL_BACKOFF_H
