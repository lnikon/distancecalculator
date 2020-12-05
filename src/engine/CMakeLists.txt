cmake_minimum_required(VERSION 3.18)
project(distancecalculator)

# If CUDA engine is supported, then include it's into build too
if(SUPPORT_CUDA_ENGINE)
add_subdirectory(cuda)
set(PROJECT_SOURCES CUDADistanceCalculatorEngine.hpp)
endif()

# Include C++ std::thread implementation
add_subdirectory(stdthreads)

set(PROJECT_SOURCES ${PROJECT_SOURCES}
                    DistanceCalculatorEngineKind.hpp 
                    IDistanceCalculatorEngine.hpp 
                    SequentialDistanceCalculatorEngine.hpp
                    CPPThreadsDistanceCalculatorEngine.hpp 
                    DistanceCalculatorEngineFactory.hpp 
                    DistanceCalculator.hpp 
                    MPIDistanceCalculatorEngine.hpp)

add_library(engine STATIC ${PROJECT_SOURCES})
target_include_directories(engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(engine PUBLIC cxx_std_17)
target_link_libraries(engine PUBLIC common structures threadpool stdthreadsengine)
 
if(SUPPORT_CUDA_ENGINE)
	target_link_libraries(engine PUBLIC CUDAMatrixDifference)
endif()
