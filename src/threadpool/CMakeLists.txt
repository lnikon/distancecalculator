cmake_minimum_required(VERSION 3.18)
project(distancecalculator)

set(PROJECT_SOURCES ThreadPool.hpp ThreadPool.cpp Semaphore.hpp)

add_library(threadpool STATIC ${PROJECT_SOURCES})
target_include_directories(threadpool PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(threadpool PUBLIC cxx_std_17)
target_link_libraries(threadpool PUBLIC common structures ${CMAKE_THREAD_LIBS_INIT})
