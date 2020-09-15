cmake_minimum_required(VERSION 3.18)
project(hello_tests)

add_executable(csvreader CSVReader_test.cpp)
target_link_libraries(csvreader PRIVATE structures readers common Catch2)
target_compile_features(csvreader PUBLIC cxx_std_17)
