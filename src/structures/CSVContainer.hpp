#pragma once

#include <memory>
#include <vector>

namespace structures
{

template <typename ValueType>
using Row = std::vector<ValueType>;

template <typename ValueType>
using RowSPtr = std::shared_ptr<Row<ValueType>>;

template <typename ValueType>
class CSVContainer
{
public:
    void append(RowSPtr<ValueType> row);

    std::size_t rowCount() const noexcept;
    std::size_t columnCount() const noexcept;

private:
    std::vector<RowSPtr<ValueType>> m_data;
};

template <typename ValueType>
void CSVContainer<ValueType>::append(RowSPtr<ValueType> row)
{
    m_data.push_back(row);
}

template <typename ValueType>
std::size_t CSVContainer<ValueType>::rowCount() const noexcept
{
    return m_data.size();
}

template <typename ValueType>
std::size_t CSVContainer<ValueType>::columnCount() const noexcept
{
    if (rowCount() >= 1)
    {
        return m_data[0]->size();
    }

    return 0;
}

template <typename ValueType>
using CSVContainerSPtr = std::shared_ptr<CSVContainer<ValueType>>;

} // namespace structures
