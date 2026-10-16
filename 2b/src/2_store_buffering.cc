#include <atomic>
#include <chrono>
#include <iostream>
#include <thread>

static std::atomic<int> x;
static std::atomic<int> y;

static int r1;
static int r2;

static void T1() {
  std::this_thread::sleep_for(std::chrono::milliseconds(1));
  x.store(1, std::memory_order_relaxed); 
  r2 = y.load(std::memory_order_relaxed); 
}

static void T2() {
  std::this_thread::sleep_for(std::chrono::milliseconds(1));
  y.store(1, std::memory_order_relaxed); 
  r1 = x.load(std::memory_order_relaxed); 
}

int main() {
  // Keep going until we see relaxed behaviour due to store buffering
  while (true) {
    x.store(0);
    y.store(0);
    r1 = 0;
    r2 = 0;
    auto t1 = std::thread(T1);
    auto t2 = std::thread(T2);
    t1.join();
    t2.join();
    std::cout << r1 << " " << r2 << std::endl;
    if (r1 == 0 && r2 == 0) {
      break;
    }
  }
}

