#include <thread> 
#include <iostream> 

static void ThreadBody1() { 
  std::cout << "Hello\n"; 
}

static void ThreadBody2() { 
  std::cout << "World"; 
}

int main() { 
  std::thread t1(ThreadBody1); 
  std::thread t2([/* capture block - empty*/](/* arguments - empty*/) -> void { 
    std::cout << "World"; 
  }); 

  t1.join(); 
  t2.join(); 
  
  return 0; 
 }

