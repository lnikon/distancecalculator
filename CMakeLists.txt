cmake_minimum_required(VERSION 3.18)
project(distancecalculator LANGUAGES CXX CUDA)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# CUDA Support
set(USE_CUDA_ENGINE OFF)
add_definitions(-DSUPPORT_CUDA_ENGINE=${USE_CUDA_ENGINE}) 

find_package(Threads)

add_subdirectory(thirdparty/Catch2)
add_subdirectory(thirdparty/benchmark)
add_subdirectory(src)
add_subdirectory(test)
# add_subdirectory(benchmark/csvreader)

message("*** CUDA BASED CALCULATION ENGINE: ${SUPPORT_CUDA_ENGINE}")
