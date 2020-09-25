#pragma once

#include "DistanceCalculatorEngineType.hpp"
#include "CSVContainer.hpp"

#include <vector>

template <typename ValueType>
class IDistanceCalculatorEngine
{
public:
    virtual structures::CSVContainerSPtr<ValueType> calculate(structures::CSVContainerSPtr<ValueType> query, structures::CSVContainerSPtr<ValueType> dataset) /* noexcept */ const = 0;
    virtual DistanceCalculatorEngineType type() /* noexcept */ const = 0;
};
