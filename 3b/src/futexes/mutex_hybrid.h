#ifndef MUTEX_HYBRID_H
#define MUTEX_HYBRID_H

#include <linux/futex.h>
#include <sys/syscall.h>
#include <unistd.h>

#include <atomic>
#include <string>

class MutexHybrid {
 public:
  MutexHybrid() {}

  void Lock() {
    // TODO
  }

  void Unlock() {
    // TODO
  }

  [[nodiscard]] static std::string GetName() { return "Hybrid"; }

 private:
  // TODO
};

#endif  // MUTEX_HYBRID_H
