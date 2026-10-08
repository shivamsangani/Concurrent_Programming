#include <atomic>
#include <functional>
#include <iostream>
#include <thread>
#include <vector>

template <typename T>
size_t Find(const std::vector<T>& data, std::function<bool(T)> predicate) {
  (void)data;
  (void)predicate;
  std::atomic<size_t> result(std::numeric_limits<size_t>::max());
  // TODO
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
      Find<int>(data, [](int item) -> bool { return item == 16000000; });

  if (result != std::numeric_limits<size_t>::max()) {
    std::cout << "Found " << data[result] << " at index " << result
              << std::endl;
  } else {
    std::cout << "Not found" << std::endl;
  }
  return 0;
}

