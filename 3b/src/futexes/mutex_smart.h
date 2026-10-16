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
    // TODO
    // syscall(SYS_futex, reinterpret_cast<int*>(&[STATE_VAR]), FUTEX_WAIT,
    // [VALUE], nullptr, nullptr, 0);
  }

  void Unlock() {
    // TODO
    // syscall(SYS_futex, reinterpret_cast<int*>(&[STATE_VAR]]), FUTEX_WAKE,
    // [VALUE], nullptr, nullptr, 0);
  }

  [[nodiscard]] static std::string GetName() { return "Smart"; }

 private:
  // TODO
};

#endif  // MUTEX_SMART_H
