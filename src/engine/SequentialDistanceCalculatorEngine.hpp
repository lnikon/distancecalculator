#pragma once

#include "IDistanceCalculatorEngine.hpp"

template <typename ValueType>
class SequentialDistanceCalculatorEngine : public IDistanceCalculatorEngine<ValueType>
{
public:
    structures::CSVContainerSPtr<ValueType> calculate(structures::CSVContainerSPtr<ValueType> query, structures::CSVContainerSPtr<ValueType> dataset) /* noexcept */ const override
    {
        return nullptr;
    }

    DistanceCalculatorEngineType type() /* noexcept */ const override
    {
        return DistanceCalculatorEngineType::Sequential;
    }
};
