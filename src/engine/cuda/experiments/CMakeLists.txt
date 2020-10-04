cmake_minimum_required(VERSION 3.18)
project(distancecalculator LANGUAGES CXX CUDA)

find_library(CUDART_LIBRARY cudart ${CMAKE_CUDA_IMPLICIT_LINK_DIRECTORIES})

set(GPU_PROJECT_SOURCES addition.cu)
add_executable(runme ${GPU_PROJECT_SOURCES})
set_target_properties(runme PROPERTIES CUDA_SEPARABLE_COMPILATION ON)
target_include_directories(runme PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CUDA_TOOLKIT_INCLUDE_DIRECTORIES})
target_link_libraries(runme ${CUDART_LIBRARY})
target_compile_features(runme PUBLIC cxx_std_17)
