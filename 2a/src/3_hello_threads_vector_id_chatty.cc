#include <cstddef>
#include <thread> 
#include <iostream> 
#include <vector>
#include <functional>

static void ThreadBody(size_t id, std::vector<std::string>& favourite_foods) { 
  std::cout << "Hello, my name is " << std::this_thread::get_id() << "but you can call me " << id << "and my favourite food is " << favourite_foods[id] << std::endl; 
}


int main() { 
  std::vector<std::thread> threads; 
  std::vector<std::string> favourite_foods = {
    "pizza", 
    "steak", 
    "broccoli", 
    "lobster", 
    "sticky toffee pudding", 
    "chicken", 
    "toad in the hole", 
    "bread pudding"
  };

  for(size_t i = 0; i < 8; i++)
  {
    threads.push_back(std::thread(ThreadBody, i, std::ref(favourite_foods)));
  }

  for(auto& thread: threads)
  {
    thread.join(); 
  }

  return 0; 

 }

