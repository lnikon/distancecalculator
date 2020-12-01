cmake_minimum_required(VERSION 3.18)
project(distancecalculator)

set(PROJECT_SOURCES Utility.cpp Utility.hpp Path.hpp Range.hpp)

add_library(common STATIC ${PROJECT_SOURCES})
target_include_directories(common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(common PUBLIC cxx_std_17)


