#ifndef SPIN_LOCK_LOCAL_SPINNING_H
#define SPIN_LOCK_LOCAL_SPINNING_H

#include <atomic>
#include <string>

class SpinLockLocalSpinning {
 public:
  SpinLockLocalSpinning() {}

  void Lock() {
    while(held_.exchange(true) == true)
    {
      //failed to acquire lock
      while(held_.load()) { 
        //spin
      }
      //when held bit is false, we exit loop and might be able to acquire the lock 
    }
    //we got lock
  }

  void Unlock() {
    held_.store(false); 
  }

  [[nodiscard]] static std::string GetName() { return "Local spinning"; }

 private:
  std::atomic<bool> held_; 
};

#endif  // SPIN_LOCK_LOCAL_SPINNING_H
