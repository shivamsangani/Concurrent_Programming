#include <thread> 
#include <iostream> 
#include <vector>

static void ThreadBody() { 
  std::cout << "Hello, my name is " << std::this_thread::get_id() << std::endl; 
}


int main() { 
  std::vector<std::thread> threads; 

  for(size_t i = 0; i < 8; i++)
  {
    threads.push_back(std::thread(ThreadBody));
  }

  for(auto& thread: threads)
  {
    thread.join(); 
  }

  return 0; 

 }

