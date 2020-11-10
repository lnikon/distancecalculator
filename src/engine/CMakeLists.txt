cmake_minimum_required(VERSION 3.18)
project(distancecalculator)

add_subdirectory(cuda)

set(PROJECT_SOURCES CUDADistanceCalculatorEngine.hpp 
                    DistanceCalculatorEngineType.hpp 
                    IDistanceCalculatorEngine.hpp 
                    SequentialDistanceCalculatorEngine.hpp
                    CPPThreadsDistanceCalculatorEngine.hpp 
                    DistanceCalculatorEngineFactory.hpp 
                    DistanceCalculator.hpp 
                    MPIDistanceCalculatorEngine.hpp)

add_library(engine STATIC ${PROJECT_SOURCES})
target_include_directories(engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(engine PUBLIC cxx_std_11)
target_link_libraries(engine PUBLIC common structures threadpool CUDAMatrixDifference)
