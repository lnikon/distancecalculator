#pragma once

#include "CPPThreadsDistanceCalculatorEngine.hpp"
#include "CUDADistanceCalculatorEngine.hpp"
#include "DistanceCalculatorEngineFactory.hpp"
#include "DistanceCalculatorEngineType.hpp"
#include "IDistanceCalculatorEngine.hpp"
#include "MPIDistanceCalculatorEngine.hpp"
#include "SequentialDistanceCalculatorEngine.hpp"
#include "CSVContainer.hpp"

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

    void calculate(structures::CSVContainerSPtr<ValueType> query, structures::CSVContainerSPtr<ValueType> dataset)
    {
        auto distances = m_engine->calculate(query, dataset);
        for (const auto& row : *distances)
        {
            for (const auto& item : *row)
            {
                std::cout << item << ' ';
            }
            std::cout << std::endl;
        }
    }

private:
    std::shared_ptr<IDistanceCalculatorEngine<ValueType>> m_engine{nullptr};
};
