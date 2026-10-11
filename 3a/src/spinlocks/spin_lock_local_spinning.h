#ifndef SPIN_LOCK_LOCAL_SPINNING_H
#define SPIN_LOCK_LOCAL_SPINNING_H

#include <atomic>
#include <string>

class SpinLockLocalSpinning {
 public:
  SpinLockLocalSpinning() {}

  void Lock() {
    // TODO
  }

  void Unlock() {
    // TODO
  }

  [[nodiscard]] static std::string GetName() { return "Local spinning"; }

 private:
  // TODO
};

#endif  // SPIN_LOCK_LOCAL_SPINNING_H
