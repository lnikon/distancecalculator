#pragma once

#include "CPPThreadsDistanceCalculatorEngine.hpp"
#include "CSVContainer.hpp"
#include "CUDADistanceCalculatorEngine.hpp"
#include "DistanceCalculatorEngineFactory.hpp"
#include "DistanceCalculatorEngineKind.hpp"
#include "IDistanceCalculatorEngine.hpp"
#include "MPIDistanceCalculatorEngine.hpp"
#include "SequentialDistanceCalculatorEngine.hpp"

#include <iostream>

template <typename ValueType, MetricKind kind = MetricKind::L2Metric>
class DistanceCalculator
{
public:
    void setDistanceCalculatorEngineKind(const DistanceCalculatorEngineKind type)
    {
        if (auto engine = distanceCalculatorEngineFactory<ValueType, kind>(type); engine != nullptr)
        {
            m_engine = engine;
        }
    }

    structures::CSVContainerSPtr<ValueType>
    calculate(structures::CSVContainerSPtr<ValueType> query,
              structures::CSVContainerSPtr<ValueType> dataset)
    {
        return m_engine->calculate(query, dataset);
    }

private:
    std::shared_ptr<IDistanceCalculatorEngine<ValueType, kind>> m_engine{nullptr};
};
