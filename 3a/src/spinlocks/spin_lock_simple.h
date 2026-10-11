#ifndef SPIN_LOCK_SIMPLE_H
#define SPIN_LOCK_SIMPLE_H

#include <atomic>
#include <string>

class SpinLockSimple {
 public:
  SpinLockSimple() {}

  void Lock() {
    // TODO
  }

  void Unlock() {
    // TODO
  }

  [[nodiscard]] static std::string GetName() { return "Simple"; }

 private:
  // TODO
};

#endif  // SPIN_LOCK_SIMPLE_H
