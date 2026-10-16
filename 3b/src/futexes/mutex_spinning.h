#ifndef MUTEX_SPINNING_H
#define MUTEX_SPINNING_H

#include <emmintrin.h>

#include <atomic>
#include <string>

class MutexSpinning {
 public:
  MutexSpinning() : lock_bit_(false) {}

  void Lock() {
    while (lock_bit_.exchange(true)) {
      do {
        for (size_t i = 0; i < 4; i++) {
          _mm_pause();
        }
      } while (lock_bit_.load());
    }
  }

  void Unlock() { lock_bit_.store(false); }

  [[nodiscard]] static std::string GetName() { return "Passive backoff"; }

 private:
  std::atomic<bool> lock_bit_;
};

#endif  // MUTEX_SPINNING_H
