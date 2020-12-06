#pragma once

#include "CPPThreadsDistanceCalculatorEngine.hpp"
#include "CUDADistanceCalculatorEngine.hpp"
#include "DistanceCalculatorEngineKind.hpp"
#include "IDistanceCalculatorEngine.hpp"
#include "MPIDistanceCalculatorEngine.hpp"
#include "SequentialDistanceCalculatorEngine.hpp"

#include <memory>

template <typename ValueType, MetricKind kind = MetricKind::L2Metric>
std::shared_ptr<IDistanceCalculatorEngine<ValueType, kind>>
distanceCalculatorEngineFactory(const EngineKind engineKind)
{
    switch (engineKind)
    {
    case EngineKind::Sequential:
        return std::make_shared<SequentialDistanceCalculatorEngine<ValueType, kind>>();
    case EngineKind::CPPThreads:
        return std::make_shared<CPPThreadsDistanceCalculatorEngine<ValueType, kind>>();
    case EngineKind::CUDA:
        return std::make_shared<CUDADistanceCalculatorEngine<ValueType, kind>>();
    case EngineKind::MPI:
        return std::make_shared<MPIDistanceCalculatorEngine<ValueType, kind>>();
    default:
        return nullptr;
    }
}
