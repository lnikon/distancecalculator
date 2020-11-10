cmake_minimum_required(VERSION 3.18)
project(distancecalculator)

set(PROJECT_SOURCES CSVContainer.cpp CSVContainer.hpp)

add_library(structures STATIC ${PROJECT_SOURCES})
target_include_directories(structures PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(structures PUBLIC cxx_std_11)
