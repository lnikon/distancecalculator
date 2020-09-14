cmake_minimum_required(VERSION 3.18)
project(distancecalculator)

set(PROJECT_SOURCES hello.cpp hello.h)

add_executable(disc main.cpp ${PROJECT_SOURCES})
add_executable(tests testmain.cpp ${PROJECT_SOURCES})
