cmake_minimum_required(VERSION 3.18)
project(distancecalculator LANGUAGES CXX CUDA)

find_library(CUDART_LIBRARY cudart ${CMAKE_CUDA_IMPLICIT_LINK_DIRECTORIES})

set(GPU_PROJECT_SOURCES MatrixDifference.cu MatrixDifference.cuh )
add_library(CUDAMatrixDifference STATIC ${GPU_PROJECT_SOURCES})
set_target_properties(CUDAMatrixDifference PROPERTIES CUDA_RESOLVE_DEVICE_SYMBOLS ON)
set_target_properties(CUDAMatrixDifference PROPERTIES CUDA_SEPARABLE_COMPILATION ON)
target_include_directories(CUDAMatrixDifference PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CUDA_TOOLKIT_INCLUDE_DIRECTORIES})
target_link_libraries(CUDAMatrixDifference ${CUDART_LIBRARY})
target_compile_features(CUDAMatrixDifference PUBLIC cxx_std_17)

add_subdirectory(experiments)
