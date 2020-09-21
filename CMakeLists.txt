cmake_minimum_required(VERSION 3.18)
project(distancecalculator)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Threads)

add_subdirectory(thirdparty/Catch2)
add_subdirectory(thirdparty/benchmark)
add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(benchmark/csvreader)
