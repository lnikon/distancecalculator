#pragma once

#include "IDistanceCalculatorEngine.hpp"

template <typename ValueType, MetricKind kind = MetricKind::L2Metric>
class SequentialDistanceCalculatorEngine : public IDistanceCalculatorEngine<ValueType, kind>
{
public:
    structures::CSVContainerSPtr<ValueType>
    calculate(structures::CSVContainerSPtr<ValueType> query,
              structures::CSVContainerSPtr<ValueType> dataset) const override
    {
		assert(query->columnCount() == dataset->rowCount());

        const auto queryRowCount      = query->rowCount();
        const auto queryColumnCount   = query->columnCount();
        const auto datasetRowCount    = dataset->rowCount();
        const auto datasetColumnCount = dataset->columnCount();
        const auto querySize          = queryRowCount * queryColumnCount;
        const auto datasetSize        = datasetRowCount * datasetColumnCount;

        const std::size_t distanceMatrixRowCnt = queryRowCount * datasetRowCount;

        auto result = std::make_shared<structures::CSVContainer<float>>();
        result->resize(distanceMatrixRowCnt, queryColumnCount);

        std::size_t whereToInsert = 0;
        for (std::size_t rowIdx = 0; rowIdx < queryRowCount; rowIdx++)
        {
            for (std::size_t datasetRowIdx = 0; datasetRowIdx < datasetRowCount; datasetRowIdx++)
            {
                for (std::size_t colIdx = 0; colIdx < queryColumnCount; colIdx++)
                {
                    result->set(whereToInsert,
                                colIdx,
                                dataset->at(datasetRowIdx, colIdx) - query->at(rowIdx, colIdx));
                }
                whereToInsert++;
            }
        }

        return result;
    }

    DistanceCalculatorEngineKind type() /* noexcept */ const override
    {
        return DistanceCalculatorEngineKind::Sequential;
    }
};
