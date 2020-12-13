cmake_minimum_required(VERSION 3.18)
project(hello_tests)

####### CSVReader Tests #######
add_executable(csvreader CSVReader_test.cpp TestConfig.hpp)
target_link_libraries(csvreader PRIVATE structures readers common Catch2)
target_compile_features(csvreader PUBLIC cxx_std_17)

####### CSVContainer Tests #######
add_executable(csvcontainer CSVContainer_test.cpp TestConfig.hpp)
target_link_libraries(csvcontainer PRIVATE structures structures common Catch2)
target_compile_features(csvcontainer PUBLIC cxx_std_17)

####### ThreadPool Tests #######
add_executable(threadpooltest ThreadPool_test.cpp TestConfig.hpp)
target_link_libraries(threadpooltest PRIVATE structures readers common threadpool Catch2)
target_compile_features(threadpooltest PUBLIC cxx_std_17)

####### CSVUtilities Tests #######
add_executable(csvutilities CSVUtilities_test.cpp TestConfig.hpp)
target_link_libraries(csvutilities PRIVATE common Catch2)
target_compile_features(csvutilities PUBLIC cxx_std_17)
