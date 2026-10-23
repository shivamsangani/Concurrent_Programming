#ifndef SPIN_LOCK_SIMPLE_H
#define SPIN_LOCK_SIMPLE_H

#include <atomic>
#include <string>

class SpinLockSimple {
 public:
  SpinLockSimple() : held_(false){}

  void Lock() {
    while (held_.exchange(true, std::memory_order_relaxed) == true)
    {
      //we have failed to acquire the lock - spin
    }
    //we have acquired the lock
    return; 
  }

  void Unlock() {
    held_.store(false, std::memory_order_relaxed); 
  }

  [[nodiscard]] static std::string GetName() { return "Simple"; }

 private:
  std::atomic<bool> held_; 
};

#endif  // SPIN_LOCK_SIMPLE_H
