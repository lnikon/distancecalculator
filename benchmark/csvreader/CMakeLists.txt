cmake_minimum_required(VERSION 3.18)
project(hello_tests)

set(BENCHMARK_ENABLE_GTEST_TESTS OFF)

add_executable(csvreaderbench main.cpp)
target_link_libraries(csvreaderbench PRIVATE structures readers common benchmark)
target_compile_features(csvreaderbench PUBLIC cxx_std_17)
