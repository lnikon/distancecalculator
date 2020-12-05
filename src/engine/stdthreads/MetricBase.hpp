#ifndef METRIC_BASE_HPP
#define METRIC_BASE_HPP

#include "../../structures/CSVContainer.hpp"

template <typename ValueType, typename MetricType>
class MetricBase
{
public:
    MetricBase()                  = default;
    MetricBase(const MetricBase&) = default;
    MetricBase(MetricBase&&)      = default;
    MetricBase& operator=(const MetricBase&) = default;
    MetricBase& operator=(MetricBase&&) = default;

    MetricBase(std::size_t                             resultIdx,
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

    template <typename F = MetricType>
    ValueType operator()()
    {
        assert(m_query->size() > 0);
        auto resultRow = std::make_shared<typename structures::Row<ValueType>>();
        resultRow->resize(m_query->size());

        for (std::size_t idx = 0; idx < m_query->size(); ++idx)
        {
            (*resultRow)[idx] = F()((*m_dataset)[idx], (*m_query)[idx]);
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

#endif
