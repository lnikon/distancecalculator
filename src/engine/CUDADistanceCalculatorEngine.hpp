#pragma once

#include "IDistanceCalculatorEngine.hpp"
#include "MatrixDifference.cuh"

template <typename ValueType>
class CUDADistanceCalculatorEngine
    : public IDistanceCalculatorEngine<ValueType> {
public:
  structures::CSVContainerSPtr<ValueType>
  calculate(structures::CSVContainerSPtr<ValueType> query,
            structures::CSVContainerSPtr<ValueType>
                dataset) /* noexcept */ const override {
    return nullptr;
  }

  DistanceCalculatorEngineType type() const /* noexcept */ override {
    return DistanceCalculatorEngineType::CUDA;
  }
};
