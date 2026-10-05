#include <iostream>
#include <functional>
#include <mutex>
#include <thread>

static void Foo(int& x) {
  std::cout << "Waking up thread 2\n";
  x = 1;
}

static void Bar(int& x, std::mutex& m) {
  std::cout << "Thread 2 waiting to be woken up\n";
  /* C++ compilers assume programs do not have undefined behaviour, which
   * allows this loop to be optimised. See the lecture and lecture slides
   * for the details. Try compiling this program at various optimisation
   * levels and inspect the assembly to see what happens. */
  while (x == 0) {}
  std::cout << "Thread 2 has been woken up!\n";
}

int main() { 
  int x = 0;
  std::mutex m;

  std::thread t1(Foo, std::ref(x));
  std::thread t2(Bar, std::ref(x), std::ref(m));

  t1.join();
  t2.join();
  return 0;
}
