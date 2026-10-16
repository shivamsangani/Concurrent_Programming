#include <thread> 
#include <iostream> 
#include <functional>

static void Foo(int& x) { 
  std::cout << "Waking up thread 2\n"; 
  x = 1; 
}

static void Bar(int& x) { 
  std::cout << "Thread 2 waiting to be woken up\n"; 
  while(x == 0) {}
  std::cout << "Thread 2 has been woken up!\n"; 
}

int main() { 
  int x = 0; 

  std::thread t1(Foo, std::ref(x)); //only need std::ref when using threads otherwise compiler can infer
  std::thread t2(Bar, std::ref(x)); 
  
  t1.join(); 
  t2.join(); 

  return 0; 
}
