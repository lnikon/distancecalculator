cmake_minimum_required(VERSION 3.18)
project(distancecalculator)

set(PROJECT_SOURCES CSVReader.cpp CSVReader.hpp)

add_library(readers STATIC ${PROJECT_SOURCES})
target_include_directories(readers PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(readers PUBLIC cxx_std_20)
target_link_libraries(readers PUBLIC common structures)


