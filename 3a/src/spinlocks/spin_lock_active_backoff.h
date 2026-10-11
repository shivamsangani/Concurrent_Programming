#ifndef SPIN_LOCK_ACTIVE_BACKOFF_H
#define SPIN_LOCK_ACTIVE_BACKOFF_H

#include <atomic>
#include <string>

class SpinLockActiveBackoff {
 public:
  SpinLockActiveBackoff() {}

  void Lock() {
    // TODO
  }

  void Unlock() {
    // TODO
  }

  [[nodiscard]] static std::string GetName() { return "Active backoff"; }

 private:
  // TODO
};

#endif  // SPIN_LOCK_ACTIVE_BACKOFF_H
