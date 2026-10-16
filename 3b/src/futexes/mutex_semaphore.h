#ifndef MUTEX_SEMAPHORE_H
#define MUTEX_SEMAPHORE_H

#include <semaphore.h>

#include <string>

// For interest, this mutex implementation uses POSIX semaphores. I wrote it
// to try to compare performance of futex and non-futex implementations.
// However, it turns out that POSIX semaphores are implemented using futexes
// (on my system, at least)!

class MutexSemaphore {
 public:
  MutexSemaphore() {
    if (sem_init(&semaphore_, 0, 1) != 0) {
      throw std::runtime_error("Problem initializing semaphore");
    }
  }

  ~MutexSemaphore() { sem_destroy(&semaphore_); }

  void Lock() {
    if (sem_wait(&semaphore_) != 0) {
      throw std::runtime_error("Problem waiting for semaphore");
    }
  }

  void Unlock() {
    if (sem_post(&semaphore_) != 0) {
      throw std::runtime_error("Problem posting to semaphore");
    }
  }

  [[nodiscard]] static std::string GetName() { return "Semaphore"; }

 private:
  sem_t semaphore_;
};

#endif  // MUTEX_SEMAPHORE_H
