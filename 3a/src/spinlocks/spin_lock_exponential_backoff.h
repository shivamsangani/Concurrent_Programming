#ifndef SPIN_LOCK_EXPONENTIAL_BACKOFF_H
#define SPIN_LOCK_EXPONENTIAL_BACKOFF_H

#include <emmintrin.h>

#include <atomic>

class SpinLockExponentialBackoff {
 public:
  SpinLockExponentialBackoff() {}

  void Lock() {
    // TODO
  }

  void Unlock() {
    // TODO
  }

  [[nodiscard]] static std::string GetName() { return "Exponential backoff"; }

 private:
  // TODO
};

#endif  // SPIN_LOCK_EXPONENTIAL_BACKOFF_H
