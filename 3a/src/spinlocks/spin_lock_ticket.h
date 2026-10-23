#ifndef SPIN_LOCK_TICKET_H
#define SPIN_LOCK_TICKET_H

#include <cstddef>
#include <emmintrin.h>

#include <atomic>
#include <string>

class SpinLockTicket {
 public:
  SpinLockTicket() : now_serving_(0), next_ticket_(0){}

  void Lock() {
    size_t ticket = next_ticket_.fetch_add(1); 
    while(now_serving_.load() != ticket)
    {
      //spin - we aren't being served
    }
    //our ticket being served, we have the lock
  }

  void Unlock() {
    //can give each of the loads and stores different memory orderings
    now_serving_.store(now_serving_.load() + 1); 
  }

  [[nodiscard]] static std::string GetName() { return "Ticket"; }

 private:
  std::atomic<size_t> now_serving_; 
  std::atomic<size_t> next_ticket_;
};

#endif  // SPIN_LOCK_TICKET_H
