#ifndef SPIN_LOCK_TICKET_H
#define SPIN_LOCK_TICKET_H

#include <emmintrin.h>

#include <atomic>
#include <string>

class SpinLockTicket {
 public:
  SpinLockTicket() {}

  void Lock() {
    // TODO
  }

  void Unlock() {
    // TODO
  }

  [[nodiscard]] static std::string GetName() { return "Ticket"; }

 private:
  // TODO
};

#endif  // SPIN_LOCK_TICKET_H
