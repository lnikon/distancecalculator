cmake_minimum_required(VERSION 3.18)
project(distancecalculator)

set(PROJECT_SOURCES hello.cpp hello.h)

add_library(hello_lib STATIC hello.cpp hello.h)
target_include_directories(hello_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(hello_lib PUBLIC cxx_std_17)

add_executable(disc main.cpp)
target_link_libraries(disc PRIVATE hello_lib engine)
target_compile_features(disc PUBLIC cxx_std_17)

add_subdirectory(common)
add_subdirectory(engine)
add_subdirectory(structures)
add_subdirectory(readers)
