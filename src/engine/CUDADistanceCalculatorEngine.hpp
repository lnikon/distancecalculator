#pragma once

#include "IDistanceCalculatorEngine.hpp"

#if SUPPORT_CUDA_ENGINE
#include "MatrixDifference.cuh"
#endif

#include <memory>
#include <cassert>

template <typename ValueType>
class CUDADistanceCalculatorEngine : public IDistanceCalculatorEngine<ValueType>
{
public:
    structures::CSVContainerSPtr<ValueType>
    calculate(structures::CSVContainerSPtr<ValueType> query,
              structures::CSVContainerSPtr<ValueType> dataset) /* noexcept */ const override
    {
#if SUPPORT_CUDA_ENGINE
		assert(query->rowCount() > 0);
		assert(dataset->rowCount() > 0);
		assert(query->columnCount() == dataset->columnCount());	

		const auto queryRowCount = query->rowCount();
		const auto queryColumnCount = query->columnCount();
		ValueType* rawQuery = query->data();

		const auto datasetRowCount = dataset->rowCount();
		const auto datasetColumnCount = dataset->columnCount();
		ValueType* rawDataset = dataset->data();

		const auto distancesRowCount = queryRowCount * datasetRowCount;
		const auto distancesColumnCount = queryColumnCount;
		ValueType* rawDistances = new ValueType[distancesRowCount * distancesColumnCount];

		L1DistanceRunner(rawQuery, rawDataset, rawDistances, queryRowCount, datasetRowCount, queryColumnCount);

		auto csvContainer = std::make_shared<typename structures::CSVContainer<float>>(rawDistances, distancesRowCount, distancesColumnCount);
		return csvContainer;
#endif
		return nullptr;
    }

    DistanceCalculatorEngineKind type() const /* noexcept */ override
    {
        return DistanceCalculatorEngineKind::CUDA;
    }
};
