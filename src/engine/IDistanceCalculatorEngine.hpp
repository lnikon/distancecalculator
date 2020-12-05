#pragma once

#include "CSVContainer.hpp"
#include "DistanceCalculatorEngineKind.hpp"
#include "DistanceCalculatorEngineMetricKind.hpp"

#include <vector>

template <typename ValueType>
class IDistanceCalculatorEngine
{
public:
    virtual structures::CSVContainerSPtr<ValueType>
                       calculate(structures::CSVContainerSPtr<ValueType> query,
                                 structures::CSVContainerSPtr<ValueType> dataset) const = 0;
    virtual EngineKind type() const = 0;
};
