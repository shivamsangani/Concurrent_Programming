#include <iostream>
#include <thread>

static void ThreadBody1() {
  std::cout << "Hello\n";
}

int main() { 
  /* Spawn two threads */
  std::thread t1(ThreadBody1);
  std::thread t2([/* capture block - empty */](/* arguments - empty */) -> void {
    /* Lambda body */
    std::cout << "World\n";
  });

  /* Threads must be joined in C++ */
  t1.join();
  t2.join();
  return 0;
}
