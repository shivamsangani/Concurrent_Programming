#ifndef MUTEX_SIMPLE_H
#define MUTEX_SIMPLE_H

#include <linux/futex.h>
#include <sys/syscall.h>
#include <unistd.h>

#include <atomic>
#include <string>

class MutexSimple {
 public:
  MutexSimple() : state_(kFree) {}

  void Lock() {
    // TODO
    while(state_.exchange(kLocked) == kLocked)
    {
      //passing the value that we get put to sleep on - kLocked
      //in case the state of the lock has changed
      syscall(SYS_futex, reinterpret_cast<int*>(&state_), FUTEX_WAIT, kLocked, nullptr, nullptr, 0); 
    }
    // syscall(SYS_futex, reinterpret_cast<int*>(&[STATE_VAR]), FUTEX_WAIT,
    // [VALUE], nullptr, nullptr, 0);
  }

  void Unlock() {
    // TODO
    state_.store(kFree); 
    syscall(SYS_futex, reinterpret_cast<int*>(&state_), FUTEX_WAKE, 1, nullptr, nullptr, 0); //1 is the number of threads we are waking up
    // syscall(SYS_futex, reinterpret_cast<int*>(&[STATE_VAR]]), FUTEX_WAKE,
    // [VALUE], nullptr, nullptr, 0);
  }

  [[nodiscard]] static std::string GetName() { return "Simple"; }

 private:
  // TODO
  const int kFree = 0; 
  const int kLocked = 1; 
  std::atomic<int> state_; 
};

#endif  // MUTEX_SIMPLE_H
