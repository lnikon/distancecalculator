cmake_minimum_required(VERSION 3.18)
project(distancecalculator LANGUAGES C CXX CUDA)

add_executable(disc main.cpp)
target_link_libraries(disc PRIVATE engine readers)
target_compile_features(disc PUBLIC cxx_std_17)

add_subdirectory(common)
add_subdirectory(engine)
add_subdirectory(structures)
add_subdirectory(readers)
add_subdirectory(threadpool)
