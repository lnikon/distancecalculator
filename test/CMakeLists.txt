cmake_minimum_required(VERSION 3.18)
project(hello_tests)

add_executable(tests testmain.cpp)
target_link_libraries(tests PRIVATE hello_lib Catch2)
target_compile_features(tests PUBLIC cxx_std_17)
