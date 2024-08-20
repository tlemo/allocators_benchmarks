
#include <memory_resource>
#include <list>
#include <cstdlib>
#include <new>
#include <cstdint>

#include <third_party/gbenchmark/benchmark.h>

namespace direct_allocation {

constexpr int COUNT = 1'000'000;

//------------------------------------------------------------------------------

void heap_malloc(benchmark::State& state) {
  static void* pointers[COUNT];
  for (auto _ : state) {
    for (int i = 0; i < COUNT; ++i) {
      auto p1 = std::malloc(sizeof(std::uint32_t));
      auto p2 = std::malloc(sizeof(std::uint32_t));
      auto p3 = std::malloc(sizeof(std::uint32_t));
      std::free(p1);
      std::free(p3);
      pointers[i] = p2;
    }
    for (int i = 0; i < COUNT; ++i) {
      std::free(pointers[i]);
    }
  }
}

BENCHMARK(heap_malloc)->Unit(benchmark::kMillisecond);

//------------------------------------------------------------------------------

void heap_new(benchmark::State& state) {
  static void* pointers[COUNT];
  for (auto _ : state) {
    for (int i = 0; i < COUNT; ++i) {
      auto p1 = ::operator new(sizeof(std::uint32_t));
      auto p2 = ::operator new(sizeof(std::uint32_t));
      auto p3 = ::operator new(sizeof(std::uint32_t));
      ::operator delete(p1);
      ::operator delete(p3);
      pointers[i] = p2;
    }
    for (int i = 0; i < COUNT; ++i) {
      ::operator delete(pointers[i]);
    }
  }
}

BENCHMARK(heap_new)->Unit(benchmark::kMillisecond);

//------------------------------------------------------------------------------

void heap_new_delete_size(benchmark::State& state) {
  static void* pointers[COUNT];
  for (auto _ : state) {
    for (int i = 0; i < COUNT; ++i) {
      auto p1 = ::operator new(sizeof(std::uint32_t));
      auto p2 = ::operator new(sizeof(std::uint32_t));
      auto p3 = ::operator new(sizeof(std::uint32_t));
      ::operator delete(p1, sizeof(std::uint32_t));
      ::operator delete(p3, sizeof(std::uint32_t));
      pointers[i] = p2;
    }
    for (int i = 0; i < COUNT; ++i) {
      ::operator delete(pointers[i], sizeof(std::uint32_t));
    }
  }
}

BENCHMARK(heap_new_delete_size)->Unit(benchmark::kMillisecond);

//------------------------------------------------------------------------------

void* new_catch(size_t size) {
  try {
    return ::operator new(size);
  } catch (...) {
    return nullptr;
  }
}

void heap_new_catch(benchmark::State& state) {
  static void* pointers[COUNT];
  for (auto _ : state) {
    for (int i = 0; i < COUNT; ++i) {
      auto p1 = new_catch(sizeof(std::uint32_t));
      auto p2 = new_catch(sizeof(std::uint32_t));
      auto p3 = new_catch(sizeof(std::uint32_t));
      ::operator delete(p1);
      ::operator delete(p3);
      pointers[i] = p2;
    }
    for (int i = 0; i < COUNT; ++i) {
      ::operator delete(pointers[i]);
    }
  }
}

BENCHMARK(heap_new_catch)->Unit(benchmark::kMillisecond);

//------------------------------------------------------------------------------

void heap_new_nothrow(benchmark::State& state) {
  static void* pointers[COUNT];
  for (auto _ : state) {
    for (int i = 0; i < COUNT; ++i) {
      auto p1 = ::operator new(sizeof(std::uint32_t), std::nothrow);
      auto p2 = ::operator new(sizeof(std::uint32_t), std::nothrow);
      auto p3 = ::operator new(sizeof(std::uint32_t), std::nothrow);
      ::operator delete(p1);
      ::operator delete(p3);
      pointers[i] = p2;
    }
    for (int i = 0; i < COUNT; ++i) {
      ::operator delete(pointers[i]);
    }
  }
}

BENCHMARK(heap_new_nothrow)->Unit(benchmark::kMillisecond);

}  // namespace direct_allocation
