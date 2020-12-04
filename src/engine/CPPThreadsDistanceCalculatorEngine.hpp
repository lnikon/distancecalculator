#pragma once

#include "IDistanceCalculatorEngine.hpp"
#include "ThreadPool.hpp"

template <typename ValueType>
class Metric
{
public:
    Metric()              = default;
    Metric(const Metric&) = default;
    Metric(Metric&&)      = default;
    Metric& operator=(const Metric&) = default;
    Metric& operator=(Metric&&) = default;

    virtual ValueType operator()() = 0;
};

template <typename ValueType>
class L1Metric
{
public:
    L1Metric()                = default;
    L1Metric(const L1Metric&) = default;
    L1Metric(L1Metric&&)      = default;
    L1Metric& operator=(const L1Metric&) = default;
    L1Metric& operator=(L1Metric&&) = default;

    L1Metric(std::size_t                             resultIdx,
             structures::RowSPtr<ValueType>          queryRow,
             structures::RowSPtr<ValueType>          datasetRow,
             structures::CSVContainerSPtr<ValueType> distances)
        : m_resultIdx(resultIdx)
        , m_query(queryRow)
        , m_dataset(datasetRow)
        , m_distances(distances)
    {
        assert(m_query->size() == m_dataset->size());
    }

    virtual ValueType operator()()
    {
        assert(m_query->size() > 0);
        auto resultRow = std::make_shared<typename structures::Row<ValueType>>();
        resultRow->resize(m_query->size());

        for (std::size_t idx = 0; idx < m_query->size(); ++idx)
        {
            (*resultRow)[idx] = (*m_dataset)[idx] - (*m_query)[idx];
        }

        m_distances->set(m_resultIdx, resultRow);
        return ValueType{};
    }

private:
    std::size_t                             m_resultIdx;
    structures::RowSPtr<ValueType>          m_query;
    structures::RowSPtr<ValueType>          m_dataset;
    structures::CSVContainerSPtr<ValueType> m_distances;
};

template <typename ValueType>
class CPPThreadsDistanceCalculatorEngine : public IDistanceCalculatorEngine<ValueType>
{
public:
    structures::CSVContainerSPtr<ValueType>
    calculate(structures::CSVContainerSPtr<ValueType> query,
              structures::CSVContainerSPtr<ValueType> dataset) /* noexcept */ const override
    {
        assert(query != nullptr);
        assert(dataset != nullptr);
        return L1Runner(query, dataset);
    }

    DistanceCalculatorEngineType type() /* noexcept */ const override
    {
        return DistanceCalculatorEngineType::CPPThreads;
    }

private:
    structures::CSVContainerSPtr<ValueType>
    L1Runner(structures::CSVContainerSPtr<ValueType> query,
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
				auto resultIdx = queryIdx * dataset->rowCount() + datasetIdx;
				if (resultIdx == 16383) {
					std::cout << "hello there\n";
				}
                pool.Submit(L1Metric<ValueType>{resultIdx,
                                                query->get(queryIdx),
                                                dataset->get(datasetIdx),
                                                result});
            }
        }

        return result;
    }
};
