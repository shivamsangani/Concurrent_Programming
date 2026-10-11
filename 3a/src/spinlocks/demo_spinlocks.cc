#include <chrono>
#include <cstddef>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

#include "src/spinlocks/spin_lock_active_backoff.h"
#include "src/spinlocks/spin_lock_exponential_backoff.h"
#include "src/spinlocks/spin_lock_local_spinning.h"
#include "src/spinlocks/spin_lock_passive_backoff.h"
#include "src/spinlocks/spin_lock_simple.h"
#include "src/spinlocks/spin_lock_ticket.h"

template <typename SpinLockType>
class SpinLockManager {
 public:
  explicit SpinLockManager(SpinLockType& lock) : lock_(lock) { lock.Lock(); }

  ~SpinLockManager() { lock_.Unlock(); }

 private:
  SpinLockType& lock_;
};

template <typename SpinLockType>
static void ThreadBodyFairness(SpinLockType& spin_lock, size_t& shared_counter,
                               size_t& increments_by_this_thread,
                               size_t max_counter_value) {
  while (true) {
    SpinLockManager<SpinLockType> lock(spin_lock);
    if (shared_counter >= max_counter_value) {
      break;
    } else {
      shared_counter++;
      increments_by_this_thread++;
    }
  }
}

template <typename SpinLockType>
static void ThreadBodyPerformance(SpinLockType& spin_lock,
                                  size_t& shared_counter,
                                  size_t increments_per_thread) {
  for (size_t i = 0; i < increments_per_thread; i++) {
    SpinLockManager<SpinLockType> lock(spin_lock);
    shared_counter++;
  }
}

template <typename SpinLockType>
static void RunBenchmarkFairness(size_t num_threads,
                                 size_t increments_per_thread_average) {
  std::cout << "Running " << SpinLockType::GetName() << " fairness test"
            << std::endl;
  auto begin_time = std::chrono::high_resolution_clock::now();
  SpinLockType spin_lock;
  size_t shared_counter = 0;
  std::vector<size_t> num_increments(num_threads, 0); 

  std::vector<std::thread> threads;
  threads.reserve(num_threads);
  for (size_t i = 0; i < num_threads; i++) {
    threads.emplace_back(ThreadBodyFairness<SpinLockType>, std::ref(spin_lock),
                         std::ref(shared_counter), std::ref(num_increments[i]),
                         increments_per_thread_average * num_threads);
  }
  for (auto& thread : threads) {
    thread.join();
  }
  auto end_time = std::chrono::high_resolution_clock::now();

  const size_t expected_value = increments_per_thread_average * num_threads;
  if (shared_counter != expected_value) {
    std::cerr << "Error: shared counter did not reach expected value "
              << expected_value << ", found " << shared_counter << std::endl;
    exit(1);
  }

  auto duration = end_time - begin_time;
  auto millis =
      std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
  std::cout << "Duration: " << millis << " ms" << std::endl;
  std::cout << "Increments per thread:" << std::endl;
  for (size_t i = 0; i < num_threads; i++) {
    std::cout << "  " << i << ": " << num_increments[i] << std::endl;
  }
  std::cout << std::endl;
}

template <typename SpinLockType>
static void RunBenchmarkPerformance(size_t num_threads,
                                    size_t increments_per_thread) {
  std::cout << "Running " << SpinLockType::GetName() << " performance test"
            << std::endl;
  auto begin_time = std::chrono::high_resolution_clock::now();
  SpinLockType spin_lock;
  size_t shared_counter = 0;
  std::vector<std::thread> threads;
  threads.reserve(num_threads);
  for (size_t i = 0; i < num_threads; i++) {
    threads.emplace_back(ThreadBodyPerformance<SpinLockType>,
                         std::ref(spin_lock), std::ref(shared_counter),
                         increments_per_thread);
  }
  for (auto& thread : threads) {
    thread.join();
  }
  auto end_time = std::chrono::high_resolution_clock::now();

  const size_t expected_value = increments_per_thread * num_threads;
  if (shared_counter != expected_value) {
    std::cerr << "Error: shared counter did not reach expected value "
              << expected_value << ", found " << shared_counter << std::endl;
    exit(1);
  }

  auto duration = end_time - begin_time;
  auto millis =
      std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
  std::cout << millis << " ms" << std::endl;
}

template <typename SpinLockType>
static void RunBenchmark(bool benchmark_is_performance, size_t num_threads,
                         size_t increments_per_thread) {
  if (benchmark_is_performance) {
    RunBenchmarkPerformance<SpinLockType>(num_threads, increments_per_thread);
  } else {
    RunBenchmarkFairness<SpinLockType>(num_threads, increments_per_thread);
  }
}

int main(int argc, char** argv) {
  if (argc != 5) {
    std::cerr << "Usage: " << argv[0]
              << R"( lock_kind benchmark_kind num_threads increments_per_thread
  lock_kind values:
     simple
     local_spinning
     active_backoff
     passive_backoff
     exp_backoff
     ticket
  benchmark_kind_values:
     performance
     fairness
)";
    return 1;
  }
  std::string lock_kind(argv[1]);
  std::string benchmark_kind(argv[2]);
  size_t num_threads = std::stoul(std::string(argv[3]));
  size_t increments_per_thread = std::stoul(std::string(argv[4]));

  bool benchmark_is_performance = false;
  if (benchmark_kind == "performance") {
    benchmark_is_performance = true;
  } else if (benchmark_kind != "fairness") {
    std::cerr << "Unknown benchmark kind: " << benchmark_kind << std::endl;
    return 1;
  }

  if (lock_kind == "simple") {
    RunBenchmark<SpinLockSimple>(benchmark_is_performance, num_threads,
                                 increments_per_thread);
  } else if (lock_kind == "local_spinning") {
    RunBenchmark<SpinLockLocalSpinning>(benchmark_is_performance, num_threads,
                                        increments_per_thread);
  } else if (lock_kind == "active_backoff") {
    RunBenchmark<SpinLockActiveBackoff>(benchmark_is_performance, num_threads,
                                        increments_per_thread);
  } else if (lock_kind == "passive_backoff") {
    RunBenchmark<SpinLockPassiveBackoff>(benchmark_is_performance, num_threads,
                                         increments_per_thread);
  } else if (lock_kind == "exp_backoff") {
    RunBenchmark<SpinLockExponentialBackoff>(
        benchmark_is_performance, num_threads, increments_per_thread);
  } else if (lock_kind == "ticket") {
    RunBenchmark<SpinLockTicket>(benchmark_is_performance, num_threads,
                                 increments_per_thread);
  } else {
    std::cerr << "Unknown lock kind: " << lock_kind << std::endl;
    return 1;
  }
}

