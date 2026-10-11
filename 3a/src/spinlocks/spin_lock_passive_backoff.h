#ifndef SPIN_LOCK_PASSIVE_BACKOFF_H
#define SPIN_LOCK_PASSIVE_BACKOFF_H

#include <emmintrin.h>

#include <atomic>
#include <string>

class SpinLockPassiveBackoff {
 public:
  SpinLockPassiveBackoff() {}

  void Lock() {
    // TODO
  }

  void Unlock() {
    // TODO
  }

  [[nodiscard]] static std::string GetName() { return "Passive backoff"; }

 private:
  // TODO
};

#endif  // SPIN_LOCK_PASSIVE_BACKOFF_H
