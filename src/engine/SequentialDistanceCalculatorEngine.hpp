#pragma once

#include "IDistanceCalculatorEngine.hpp"

template <typename ValueType>
class SequentialDistanceCalculatorEngine : public IDistanceCalculatorEngine<ValueType>
{
public:
    structures::CSVContainerSPtr<ValueType>
    calculate(structures::CSVContainerSPtr<ValueType> query,
              structures::CSVContainerSPtr<ValueType> dataset) /* noexcept */ const override
    {
        auto              result = std::make_shared<structures::CSVContainer<float>>();
        const std::size_t distanceMatrixRowCnt = query->rowCount() * dataset->rowCount();
        result->resize(distanceMatrixRowCnt);

        // Using simple L1 norm
        const auto  queryRowCount      = query->rowCount();
        const auto  queryColumnCount   = query->columnCount();
        const auto  datasetRowCount    = dataset->rowCount();
        const auto  datasetColumnCount = dataset->columnCount();
        std::size_t whereToInsert      = 0;
        for (std::size_t rowIdx = 0; rowIdx < queryRowCount; rowIdx++)
        {
            for (std::size_t datasetRowIdx = 0; datasetRowIdx < datasetRowCount; datasetRowIdx++)
            {
                auto resultRow = std::make_shared<structures::Row<ValueType>>();
                resultRow->resize(queryColumnCount);
                for (std::size_t colIdx = 0; colIdx < queryColumnCount; colIdx++)
                {
                    (*resultRow)[colIdx] =
                        (*dataset->get(datasetRowIdx))[colIdx] - (*query->get(rowIdx))[colIdx];
                }
                result->set(whereToInsert, std::move(resultRow));
                whereToInsert++;
            }
        }

        return result;
    }

    DistanceCalculatorEngineType type() /* noexcept */ const override
    {
        return DistanceCalculatorEngineType::Sequential;
    }
};
