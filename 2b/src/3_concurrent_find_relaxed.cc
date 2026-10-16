#include <atomic>
#include <cstddef>
#include <functional>
#include <iostream>
#include <limits>
#include <thread>
#include <vector>

template <typename T> 
size_t Find(const std::vector<T>& data, std::function<bool(T)> predicate) { 
  (void)data; 
  (void)predicate; 
  std::atomic<size_t> result(std::numeric_limits<size_t>::max()); //return max value of size_t
  //TODO
  std::function<void(size_t, size_t)> threadBody = [&predicate, &data, &result](size_t start, size_t end) -> void { 
    for(size_t i = start; i < end; i++)
    {
      if(predicate(data[i]))
      {
        //only ever storing to result twice, so stores are not causing long execution time
        //iterating over all numbers its what's taking long - so optimising store is not deep
        result.store(i, std::memory_order_relaxed); 
        return; 
      }
    }
  };

  std::thread t1(threadBody, 0, data.size()/2); 
  std::thread t2(threadBody, data.size()/2, data.size()); 

  t1.join(); 
  t2.join(); 

  return result; 
}

int main() {
  std::vector<int> data;
  for (size_t count = 0; count < 2; count++) {
    for (int i = 0; i < (1 << 24); i++) {
      data.push_back(i);
    }
  }
  size_t result =
      Find<int>(data, [](int item) -> bool { return item == 5; });

  if (result != std::numeric_limits<size_t>::max()) {
    std::cout << "Found " << data[result] << " at index " << result
              << std::endl;
  } else {
    std::cout << "Not found" << std::endl;
  }
  return 0;
}

