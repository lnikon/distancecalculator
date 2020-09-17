#pragma once

#include "CPPThreadsDistanceCalculatorEngine.hpp"
#include "CUDADistanceCalculatorEngine.hpp"
#include "DistanceCalculatorEngineFactory.hpp"
#include "DistanceCalculatorEngineType.hpp"
#include "IDistanceCalculatorEngine.hpp"
#include "MPIDistanceCalculatorEngine.hpp"
#include "SequentialDistanceCalculatorEngine.hpp"

#include <iostream>

template <typename ValueType>
class DistanceCalculator
{
public:
    void setDistanceCalculatorEngineType(const DistanceCalculatorEngineType type)
    {
        if (auto engine = distanceCalculatorEngineFactory<ValueType>(type); engine != nullptr)
        {
            m_engine = engine;
        }
    }

    void calculate()
    {
        std::cout << "DistanceCalculator::calculate()\n";
    }

private:
    std::shared_ptr<IDistanceCalculatorEngine<ValueType>> m_engine{nullptr};
};
