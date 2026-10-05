#include <iostream>
#include <thread>
#include <vector>

static void ThreadBody() {
  /* std::this_thread::get_id() gets the unique ID associated with a thread.
  *  This ID is not very useful in practice. */
  std::cout << "Hello, my name is " << std::this_thread::get_id() << "\n";
}

int main() { 
  std::vector<std::thread> threads;

  for (size_t i = 0; i < 8; i++) {
    threads.push_back(std::thread(ThreadBody));
  }

  for (auto& thread : threads) {
    thread.join();
  }

  return 0;
}
