#pragma once

#include "DistanceCalculatorEngineMetricKind.hpp"
#include "IDistanceCalculatorEngine.hpp"
#include "ThreadPool.hpp"
#include "stdthreads/HammingDistance.hpp"
#include "stdthreads/LNMetric.hpp"

#include <cmath>

template <typename ValueType, MetricKind kind>
struct Submitter
{
    void operator()(ThreadPool&                             pool,
                    std::size_t                             resultIdx,
                    structures::RowSPtr<ValueType>          queryRow,
                    structures::RowSPtr<ValueType>          datasetRow,
                    structures::CSVContainerSPtr<ValueType> distances)
    {
        Submitter<ValueType, kind>(pool, resultIdx, queryRow, datasetRow, distances);
    }
};

template <typename ValueType>
struct Submitter<ValueType, MetricKind::L1Metric>
{
    void operator()(ThreadPool&                             pool,
                    std::size_t                             resultIdx,
                    structures::RowSPtr<ValueType>          queryRow,
                    structures::RowSPtr<ValueType>          datasetRow,
                    structures::CSVContainerSPtr<ValueType> distances)
    {
        pool.Submit(L1Metric<ValueType>{resultIdx, queryRow, datasetRow, distances});
    }
};

template <typename ValueType>
struct Submitter<ValueType, MetricKind::L2Metric>
{
    void operator()(ThreadPool&                             pool,
                    std::size_t                             resultIdx,
                    structures::RowSPtr<ValueType>          queryRow,
                    structures::RowSPtr<ValueType>          datasetRow,
                    structures::CSVContainerSPtr<ValueType> distances)
    {
        pool.Submit(L2Metric<ValueType>{resultIdx, queryRow, datasetRow, distances});
    }
};

template <typename ValueType>
struct Submitter<ValueType, MetricKind::HammingDistance>
{
    void operator()(ThreadPool&                             pool,
                    std::size_t                             resultIdx,
                    structures::RowSPtr<ValueType>          queryRow,
                    structures::RowSPtr<ValueType>          datasetRow,
                    structures::CSVContainerSPtr<ValueType> distances)
    {
        pool.Submit(HammingDistance<ValueType>{resultIdx, queryRow, datasetRow, distances});
    }
};

template <typename ValueType, MetricKind kind=MetricKind::L1Metric>
class CPPThreadsDistanceCalculatorEngine : public IDistanceCalculatorEngine<ValueType>
{
public:
    structures::CSVContainerSPtr<ValueType>
    calculate(structures::CSVContainerSPtr<ValueType> query,
              structures::CSVContainerSPtr<ValueType> dataset) /* noexcept */ const override
    {
        assert(query != nullptr);
        assert(dataset != nullptr);
		return runner(query, dataset);
    }

    EngineKind type() const override
    {
        return EngineKind::CPPThreads;
    }

private:
    structures::CSVContainerSPtr<ValueType>
    runner(structures::CSVContainerSPtr<ValueType> query,
           structures::CSVContainerSPtr<ValueType> dataset) const
    {
        const auto distanceMatrixRowCnt = query->rowCount() * dataset->rowCount();
        auto       result               = std::make_shared<structures::CSVContainer<float>>();
        result->resize(distanceMatrixRowCnt);

        ThreadPool pool;
        for (std::size_t queryIdx : range(query->rowCount()))
        {
            for (std::size_t datasetIdx : range(dataset->rowCount()))
            {
                const auto resultIdx = queryIdx * dataset->rowCount() + datasetIdx;
                Submitter<ValueType, kind>{}(
                    pool, resultIdx, query->get(queryIdx), dataset->get(datasetIdx), result);
            }
        }

        return result;
    }

};
