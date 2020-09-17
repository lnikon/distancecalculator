#pragma once

#include "DistanceCalculatorEngineType.hpp"

#include <vector>

template <typename ValueType>
class IDistanceCalculatorEngine
{
public:
    virtual std::vector<std::vector<ValueType>> calculate() /* noexcept */ const = 0;
    virtual DistanceCalculatorEngineType type() /* noexcept */ const = 0;
};
