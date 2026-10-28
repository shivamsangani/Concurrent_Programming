#ifndef MUTEX_SMART_H
#define MUTEX_SMART_H

#include <linux/futex.h>
#include <sys/syscall.h>
#include <unistd.h>

#include <atomic>
#include <string>

class MutexSmart {
 public:
  MutexSmart() {}

  void Lock() {
    int old_value = cmpxchg(kFree, kLockedNoWaiters); 
    if(old_value == kFree)
    {
      //We got the lock on the fast path 
      return; 
    }

    do { 
      if(old_value == kLockedWaiters || cmpxchg(kLockedNoWaiters, kLockedWaiters) != kFree)
      {
        //Only going to go to sleep unless other threads KNOW that we are sleeping - not going to be woken up otherwise
        syscall(SYS_futex, reinterpret_cast<int*>(&state_), FUTEX_WAIT, kLockedWaiters, nullptr, nullptr, 0);   
      }


      old_value = cmpxchg(kFree, kLockedWaiters); 
    } while (old_value != kFree);

    // syscall(SYS_futex, reinterpret_cast<int*>(&[STATE_VAR]), FUTEX_WAIT,
    // [VALUE], nullptr, nullptr, 0);
  }

  void Unlock() {
    if(state_.exchange(kFree) == kLockedWaiters)
    {
      syscall(SYS_futex, reinterpret_cast<int*>(&state_), FUTEX_WAKE, 1, nullptr, nullptr, 0); 
    }
    // syscall(SYS_futex, reinterpret_cast<int*>(&[STATE_VAR]]), FUTEX_WAKE,
    // [VALUE], nullptr, nullptr, 0);
  }

  [[nodiscard]] static std::string GetName() { return "Smart"; }

 private:
  const int kFree = 0; 
  //the mutex is locked and we dont believe there are waiters but there may really be
  const int kLockedNoWaiters = 1; 
  //the mutex is locked and we believe there are waiters but there may not be 
  const int kLockedWaiters = 2; 

  std::atomic<int> state_; 
  
  int cmpxchg(int expected, int desired) { 
    state_.compare_exchange_strong(expected, desired); 
    return expected; 
  }
};

#endif  // MUTEX_SMART_H
