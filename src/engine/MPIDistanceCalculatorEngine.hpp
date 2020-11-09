#pragma once

#include "IDistanceCalculatorEngine.hpp"

template <typename ValueType>
class MPIDistanceCalculatorEngine
    : public IDistanceCalculatorEngine<ValueType> {
public:
  struct Options {};

  explicit MPIDistanceCalculatorEngine() = default;

  explicit MPIDistanceCalculatorEngine(const Options &options)
      : m_options(options) {}

  structures::CSVContainerSPtr<ValueType>
  calculate(structures::CSVContainerSPtr<ValueType> query,
            structures::CSVContainerSPtr<ValueType>
                dataset) /* noexcept */ const override {
    return nullptr;
  }

  DistanceCalculatorEngineType type() /* noexcept */ const override {
    return DistanceCalculatorEngineType::MPI;
  }

private:
  Options m_options;
};
