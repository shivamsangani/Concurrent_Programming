#ifndef MUTEX_SIMPLE_H
#define MUTEX_SIMPLE_H

#include <linux/futex.h>
#include <sys/syscall.h>
#include <unistd.h>

#include <atomic>
#include <string>

class MutexSimple {
 public:
  MutexSimple() {}

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

  [[nodiscard]] static std::string GetName() { return "Simple"; }

 private:
  // TODO
};

#endif  // MUTEX_SIMPLE_H
