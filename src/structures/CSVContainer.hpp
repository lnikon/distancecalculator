#pragma once

#include "Range.hpp"

#include <cassert>
#include <iostream>
#include <memory>
#include <shared_mutex>
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
    CSVContainer()
    {
    }

    CSVContainer(const CSVContainer&) = default;
    CSVContainer(CSVContainer&&)      = default;

    CSVContainer& operator=(const CSVContainer&) = default;
    CSVContainer& operator=(CSVContainer&&) = default;

    ~CSVContainer() = default;

    CSVContainer(ValueType* T, const std::size_t rowDim, const std::size_t colDim);

    void append(RowSPtr<ValueType> row);

    std::size_t rowCount() const noexcept;
    std::size_t columnCount() const noexcept;

    typename std::vector<RowSPtr<ValueType>>::iterator begin();
    typename std::vector<RowSPtr<ValueType>>::iterator end();

    typename std::vector<RowSPtr<ValueType>>::const_iterator cbegin();
    typename std::vector<RowSPtr<ValueType>>::const_iterator cend();

    void print()
    {
        for (auto row : range(rowCount()))
        {
            for (auto col : range(columnCount()))
            {
                std::cout << m_data[row][col] << ' ';
            }
            std::cout << '\n';
        }
        std::cout << '\n';
    }

    RowSPtr<ValueType> get(const std::size_t idx) const
    {
        std::shared_lock slck(m_mutex);
        assert(idx < m_data.size());
        return m_data[idx];
    }

    ValueType item(const std::size_t row, const std::size_t col) const
    {
        std::shared_lock slck(m_mutex);
        assert(row < rowCount());
        assert(col < columnCount());
        return (*m_data[row])[col];
    }

    void set(const std::size_t idx, RowSPtr<ValueType> value)
    {
		std::lock_guard lg(m_mutex);
        assert(idx < m_data.size());
        m_data[idx] = value;
    }

    void resize(const std::size_t rowDim, const std::size_t colDim = 0);

private:
    mutable std::shared_mutex       m_mutex;
    std::vector<RowSPtr<ValueType>> m_data{};
};

template <typename ValueType>
CSVContainer<ValueType>::CSVContainer(
    ValueType*        aPtr,   /* Pointer to the array representation of a matrix */
    const std::size_t rowDim, /* Number of rows */
    const std::size_t colDim) /* Number of columns */
{
    assert(aPtr != 0);
    assert(rowDim != 0);
    assert(colDim != 0);

    resize(rowDim, colDim);
    for (std::size_t row : range(rowDim))
    {
        for (std::size_t col : range(colDim))
        {
            (*m_data[row])[col] = *(aPtr + row * columnCount() + col);
        }
    }
}

template <typename ValueType>
void CSVContainer<ValueType>::append(RowSPtr<ValueType> row)
{
	std::lock_guard lg(m_mutex);
    if (row->size() > columnCount())
    {
        for (auto& ptr : m_data)
        {
            ptr->resize(row->size());
        }
    }

    m_data.push_back(row);
}

template <typename ValueType>
std::size_t CSVContainer<ValueType>::rowCount() const noexcept
{
	std::shared_lock slck(m_mutex);
    return m_data.size();
}

template <typename ValueType>
std::size_t CSVContainer<ValueType>::columnCount() const noexcept
{
	std::shared_lock slck(m_mutex);
    if (rowCount() >= 1)
    {
        if (m_data[0] == nullptr)
        {
            return 0;
        }

        return m_data[0]->size();
    }

    return 0;
}

template <typename ValueType>
void CSVContainer<ValueType>::resize(const std::size_t rowDim, const std::size_t colDim)
{
	std::lock_guard lg(m_mutex);
    assert(rowDim > 0);
    m_data.clear();

    m_data.resize(rowDim);
    for (auto& ptr : m_data)
    {
        ptr = std::make_shared<typename structures::Row<ValueType>>();
    }

    if (colDim > 0)
    {
        for (auto& ptr : m_data)
        {
            ptr->resize(colDim);
        }
    }
}

/*
 * Members
 */
template <typename ValueType>
typename std::vector<RowSPtr<ValueType>>::iterator CSVContainer<ValueType>::begin()
{
	std::shared_lock slck(m_mutex);
    return m_data.begin();
}

template <typename ValueType>
typename std::vector<RowSPtr<ValueType>>::iterator CSVContainer<ValueType>::end()
{
	std::shared_lock slck(m_mutex);
    return m_data.end();
}

template <typename ValueType>
typename std::vector<RowSPtr<ValueType>>::const_iterator CSVContainer<ValueType>::cbegin()
{
	std::shared_lock slck(m_mutex);
    return m_data.cbegin();
}

template <typename ValueType>
typename std::vector<RowSPtr<ValueType>>::const_iterator CSVContainer<ValueType>::cend()
{
	std::shared_lock slck(m_mutex);
    return m_data.cend();
}

/*
 * Free functions.
 */
template <typename ValueType>
typename std::vector<RowSPtr<ValueType>>::iterator begin(CSVContainer<ValueType>& c)
{
    return c.begin();
}

template <typename ValueType>
typename std::vector<RowSPtr<ValueType>>::iterator end(CSVContainer<ValueType>& c)
{
    return c.end();
}

template <typename ValueType>
typename std::vector<RowSPtr<ValueType>>::const_iterator cbegin(CSVContainer<ValueType>& c)
{
    return c.cbegin();
}

template <typename ValueType>
typename std::vector<RowSPtr<ValueType>>::const_iterator cend(CSVContainer<ValueType>& c)
{
    return c.cend();
}

template <typename ValueType>
using CSVContainerSPtr = std::shared_ptr<CSVContainer<ValueType>>;

} // namespace structures
