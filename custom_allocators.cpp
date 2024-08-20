
#include <memory_resource>
#include <list>
#include <cstdlib>
#include <new>

#include <third_party/gbenchmark/benchmark.h>

namespace custom_allocators {

constexpr int COUNT = 1'000'000;

//------------------------------------------------------------------------------

void ca_pmr(benchmark::State& state) {
  for (auto _ : state) {
    std::pmr::list<char> c;
    for (int i = 0; i < COUNT; ++i) {
      c.emplace_back('a');
    }
    benchmark::DoNotOptimize(c);
  }
}

BENCHMARK(ca_pmr)->Unit(benchmark::kMillisecond);

//------------------------------------------------------------------------------

void ca_std_default(benchmark::State& state) {
  for (auto _ : state) {
    std::list<char> c;
    for (int i = 0; i < COUNT; ++i) {
      c.emplace_back('a');
    }
    benchmark::DoNotOptimize(c);
  }
}

BENCHMARK(ca_std_default)->Unit(benchmark::kMillisecond);

//------------------------------------------------------------------------------

template <class T>
struct malloc_allocator {
  using value_type = T;
  T* allocate(size_t count) noexcept {
    return static_cast<T*>(std::malloc(count * sizeof(T)));
  }
  void deallocate(T* ptr, size_t) noexcept { std::free(ptr); }
  bool operator==(const malloc_allocator<T>& other) const noexcept {
    return true;
  }
};

void ca_std_malloc(benchmark::State& state) {
  for (auto _ : state) {
    std::list<char, malloc_allocator<char>> c;
    for (int i = 0; i < COUNT; ++i) {
      c.emplace_back('a');
    }
    benchmark::DoNotOptimize(c);
  }
}

BENCHMARK(ca_std_malloc)->Unit(benchmark::kMillisecond);

//------------------------------------------------------------------------------

template <class T>
struct new_allocator {
  using value_type = T;
  T* allocate(size_t count) {
    return static_cast<T*>(::operator new(count * sizeof(T)));
  }
  void deallocate(T* ptr, size_t) noexcept { ::operator delete(ptr); }
  bool operator==(const new_allocator<T>& other) const noexcept { return true; }
};

void ca_std_new(benchmark::State& state) {
  for (auto _ : state) {
    std::list<char, new_allocator<char>> c;
    for (int i = 0; i < COUNT; ++i) {
      c.emplace_back('a');
    }
    benchmark::DoNotOptimize(c);
  }
}

BENCHMARK(ca_std_new)->Unit(benchmark::kMillisecond);

//------------------------------------------------------------------------------

template <class T>
struct noexcept_new_allocator {
  using value_type = T;
  T* allocate(size_t count) noexcept {
    return static_cast<T*>(::operator new(count * sizeof(T), std::nothrow));
  }
  void deallocate(T* ptr, size_t) noexcept { ::operator delete(ptr); }
  bool operator==(const noexcept_new_allocator<T>& other) const noexcept {
    return true;
  }
};

void ca_std_noexcept_new(benchmark::State& state) {
  for (auto _ : state) {
    std::list<char, noexcept_new_allocator<char>> c;
    for (int i = 0; i < COUNT; ++i) {
      c.emplace_back('a');
    }
    benchmark::DoNotOptimize(c);
  }
}
BENCHMARK(ca_std_noexcept_new)->Unit(benchmark::kMillisecond);

}  // namespace custom_allocators
