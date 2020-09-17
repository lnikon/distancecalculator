#pragma once

#include "IDistanceCalculatorEngine.hpp"

template <typename ValueType>
class CUDADistanceCalculatorEngine : public IDistanceCalculatorEngine<ValueType>
{
public:
    std::vector<std::vector<ValueType>> calculate() /* noexcept */ const override
    {
        return std::vector<std::vector<ValueType>>{};
    }

    DistanceCalculatorEngineType type() const /* noexcept */ override
    {
        return DistanceCalculatorEngineType::CUDA;
    }
};
