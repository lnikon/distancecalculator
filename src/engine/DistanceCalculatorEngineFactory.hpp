#pragma once

#include "CPPThreadsDistanceCalculatorEngine.hpp"
#include "CUDADistanceCalculatorEngine.hpp"
#include "DistanceCalculatorEngineType.hpp"
#include "IDistanceCalculatorEngine.hpp"
#include "MPIDistanceCalculatorEngine.hpp"
#include "SequentialDistanceCalculatorEngine.hpp"

#include <memory>

template <typename ValueType>
std::shared_ptr<IDistanceCalculatorEngine<ValueType>>
distanceCalculatorEngineFactory(const DistanceCalculatorEngineType type)
{
    switch (type)
    {
    case DistanceCalculatorEngineType::Sequential:
        return std::make_shared<SequentialDistanceCalculatorEngine<ValueType>>();
    case DistanceCalculatorEngineType::CPPThreads:
        return std::make_shared<CPPThreadsDistanceCalculatorEngine<ValueType>>();
    case DistanceCalculatorEngineType::CUDA:
        return std::make_shared<CUDADistanceCalculatorEngine<ValueType>>();
    case DistanceCalculatorEngineType::MPI:
        return std::make_shared<MPIDistanceCalculatorEngine<ValueType>>();
    default:
        return nullptr;
    }
}
