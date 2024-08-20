
cmake_minimum_required(VERSION 3.25 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 20)

# Default to Release builds
if(NOT CMAKE_BUILD_TYPE)
    message("Build type not specified, using Release")
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type not specified, using Release" FORCE)
endif(NOT CMAKE_BUILD_TYPE)

project(benchmarks LANGUAGES C CXX)

include_directories(${benchmarks_SOURCE_DIR})

add_subdirectory(third_party/gbenchmark)

add_executable(benchmarks
    custom_allocators.cpp
    direct_heap.cpp
    main.cpp)

target_link_libraries(benchmarks PRIVATE benchmark)
