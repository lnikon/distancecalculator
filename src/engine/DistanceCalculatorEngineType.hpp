#pragma once

enum class DistanceCalculatorEngineType
{
    Sequential = 0,
    CPPThreads,
    CUDA,
    MPI
};
