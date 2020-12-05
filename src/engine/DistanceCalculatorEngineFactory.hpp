#pragma once

#include "CPPThreadsDistanceCalculatorEngine.hpp"
#include "CUDADistanceCalculatorEngine.hpp"
#include "DistanceCalculatorEngineKind.hpp"
#include "IDistanceCalculatorEngine.hpp"
#include "MPIDistanceCalculatorEngine.hpp"
#include "SequentialDistanceCalculatorEngine.hpp"

#include <memory>

template <typename ValueType>
std::shared_ptr<IDistanceCalculatorEngine<ValueType>>
distanceCalculatorEngineFactory(const EngineKind engineKind)
{
    switch (engineKind)
    {
    case EngineKind::Sequential:
        return std::make_shared<SequentialDistanceCalculatorEngine<ValueType>>();
    case EngineKind::CPPThreads:
        return std::make_shared<CPPThreadsDistanceCalculatorEngine<ValueType>>();
    case EngineKind::CUDA:
        return std::make_shared<CUDADistanceCalculatorEngine<ValueType>>();
    case EngineKind::MPI:
        return std::make_shared<MPIDistanceCalculatorEngine<ValueType>>();
    default:
        return nullptr;
    }
}
