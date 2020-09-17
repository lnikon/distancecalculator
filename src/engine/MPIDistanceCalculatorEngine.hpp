#pragma once

#include "IDistanceCalculatorEngine.hpp"

template <typename ValueType>
class MPIDistanceCalculatorEngine : public IDistanceCalculatorEngine<ValueType>
{
public:
    struct Options
    {
    };

    explicit MPIDistanceCalculatorEngine() = default;

    explicit MPIDistanceCalculatorEngine(const Options& options)
        : m_options(options)
    {
    }

    std::vector<std::vector<ValueType>> calculate() /* noexcept */ const override
    {
        return std::vector<std::vector<ValueType>>{};
    }

    DistanceCalculatorEngineType type() /* noexcept */ const override
    {
        return DistanceCalculatorEngineType::MPI;
    }

private:
    Options m_options;
};
