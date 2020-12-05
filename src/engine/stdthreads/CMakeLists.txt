cmake_minimum_required(VERSION 3.18)
project(distancecalculator LANGUAGES CXX)

set(STD_THREADS_SOURCES
	LNMetric.hpp
	HammingDistance.hpp
	MetricBase.hpp)

add_library(stdthreadsengine STATIC ${STD_THREADS_SOURCES})
target_include_directories(stdthreadsengine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(stdthreadsengine PUBLIC cxx_std_17)
target_link_libraries(stdthreadsengine PUBLIC common structures threadpool)
