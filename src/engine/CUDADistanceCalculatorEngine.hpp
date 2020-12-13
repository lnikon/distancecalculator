#pragma once

#include "DistanceCalculatorEngineMetricKind.hpp"
#include "IDistanceCalculatorEngine.hpp"
#include "stdthreads/HammingDistance.hpp"
#include "stdthreads/LNMetric.hpp"
#include "stdthreads/MetricBase.hpp"

#include "MatrixDifference.cuh"

#include <cassert>
#include <iostream>
#include <memory>

template <typename ValueType, MetricKind kind = MetricKind::L2Metric>
class CUDADistanceCalculatorEngine : public IDistanceCalculatorEngine<ValueType, kind>
{
public:
    structures::CSVContainerSPtr<ValueType>
    calculate(structures::CSVContainerSPtr<ValueType> query,
              structures::CSVContainerSPtr<ValueType> dataset) /* noexcept */ const override
    {
        assert(query->rowCount() > 0);
        assert(dataset->rowCount() > 0);
        assert(query->columnCount() == dataset->columnCount());

        const auto queryRowCount    = query->rowCount();
        const auto queryColumnCount = query->columnCount();
        ValueType* rawQuery         = query->data();

        const auto datasetRowCount    = dataset->rowCount();
        const auto datasetColumnCount = dataset->columnCount();
        ValueType* rawDataset         = dataset->data();

        const auto distancesRowCount    = queryRowCount * datasetRowCount;
        const auto distancesColumnCount = queryColumnCount;
        ValueType* rawDistances         = new ValueType[distancesRowCount * distancesColumnCount];

        switch (kind)
        {
        case MetricKind::L1Metric:
            L1DistanceRunner(rawQuery,
                             rawDataset,
                             rawDistances,
                             queryRowCount,
                             datasetRowCount,
                             queryColumnCount);
            break;
        case MetricKind::L2Metric:
            L2DistanceRunner(rawQuery,
                             rawDataset,
                             rawDistances,
                             queryRowCount,
                             datasetRowCount,
                             queryColumnCount);
            break;
        case MetricKind::HammingDistance:
            HammingDistanceRunner(rawQuery,
                                  rawDataset,
                                  rawDistances,
                                  queryRowCount,
                                  datasetRowCount,
                                  queryColumnCount);
            break;
        default:
            return nullptr;
        }

        auto csvContainer = std::make_shared<typename structures::CSVContainer<float>>(
            rawDistances, distancesRowCount, distancesColumnCount);
        return csvContainer;
    }

    DistanceCalculatorEngineKind type() const /* noexcept */ override
    {
        return DistanceCalculatorEngineKind::CUDA;
    }
};
