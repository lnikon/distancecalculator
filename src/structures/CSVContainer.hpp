#pragma once

#include "../common/Range.hpp"

#include <cassert>
#include <iostream>
#include <memory>
#include <shared_mutex>
#include <vector>

namespace structures
{

template <typename ValueType>
class NonOwningArray : public std::enable_shared_from_this<NonOwningArray<ValueType>>
{
public:
	template <typename ItValueType = ValueType>
	struct NonOwningArrayIterator : public std::iterator<std::forward_iterator_tag, ValueType> {
		friend class NonOwningArray<ValueType>;

		public:
		ItValueType& operator*()
		{
			return *m_current;
		}

		const NonOwningArrayIterator<ItValueType>& operator++()
		{
			m_current++;
			return *this;
		}
		
		bool operator!=(const NonOwningArrayIterator<ItValueType>& other) const
		{
			return m_current != other.m_current;
		}

		private:
		ValueType* m_current;
		NonOwningArrayIterator(ValueType* current) : m_current(current) {}
	};

	using iterator = NonOwningArrayIterator<ValueType>;
	using const_iterator = const NonOwningArrayIterator<ValueType>;

    NonOwningArray(ValueType* ptr, const std::size_t size)
        : m_ptr(ptr)
        , m_size(size)
    {
    }

	std::size_t size() const
	{
		return m_size;
	}

	ValueType& operator[](const std::size_t idx)
	{
		assert(idx < m_size);
		return *(m_ptr + idx);
	}

	iterator begin()
	{
		return NonOwningArrayIterator<ValueType>(m_ptr);
	}

	iterator end()
	{
		return NonOwningArrayIterator<ValueType>(nullptr);
	}
	
	const_iterator cbegin()
	{
		return NonOwningArrayIterator<ValueType>(m_ptr);
	}

	iterator cend()
	{
		return NonOwningArrayIterator<ValueType>(nullptr);
	}

private:
    ValueType*        m_ptr{nullptr};
    const std::size_t m_size;
};

template <typename ValueType>
using Row = NonOwningArray<ValueType>;

template <typename ValueType>
using RowSPtr = std::shared_ptr<Row<ValueType>>;

template <typename ValueType>
class Flat2DMatrix
{
public:
    Flat2DMatrix()                    = default;
    Flat2DMatrix(const Flat2DMatrix&) = default;
    Flat2DMatrix(Flat2DMatrix&&)      = default;
    Flat2DMatrix& operator=(const Flat2DMatrix&) = default;
    Flat2DMatrix& operator=(Flat2DMatrix&&) = default;

    /**
     * @brief Return value stored at row in a given column
     */
    const ValueType& at(const std::size_t row, const std::size_t column) const
    {
        assert(row < m_rowCount);
        assert(column < m_columnCount);
        return *(m_data.data() + row * m_columnCount + column);
    }

    ValueType& at(const std::size_t row, const std::size_t column)
    {
        return const_cast<ValueType&>(
            static_cast<const Flat2DMatrix<ValueType>*>(this)->at(row, column));
    }

    ValueType* data()
    {
        return m_data.data();
    }

    bool empty() const
    {
        return (rowCount() != 0) && (columnCount() != 0);
    }

    void clear()
    {
        m_data.clear();
    }

    void resize(const std::size_t rowCount, const std::size_t columnCount)
    {
        assert(rowCount > 0);
        assert(columnCount > 0);

        std::cout << "row=" << rowCount << "; column=" << columnCount << ";\n";

        // This part is little bit tricky
        // Sometimes client may want to allocate @rowCount rows without specifying @columnCount
        // So we need to handle this case explicitly
        if (columnCount > 0)
        {
            const std::size_t sz{rowCount * columnCount};
            m_data.resize(sz);
        }

        // When construction went successfully, update row and column count
        m_rowCount    = rowCount;
        m_columnCount = columnCount;
    }

    void set(const std::size_t row, const std::size_t column, const ValueType& value)
    {
        at(row, column) = value;
    }

	NonOwningArray<ValueType> row(const std::size_t rowIdx)
	{
		return NonOwningArray<ValueType>(data() + rowIdx*columnCount(), columnCount());
	}

    std::size_t rowCount() const
    {
        return m_rowCount;
    }

    std::size_t columnCount() const
    {
        return m_columnCount;
    }
	
private:
    std::size_t            m_rowCount{0};
    std::size_t            m_columnCount{0};
    std::vector<ValueType> m_data{};
};

template <typename ValueType>
class CSVContainer
{
public:
    CSVContainer()                    = default;
    CSVContainer(const CSVContainer&) = default;
    CSVContainer(CSVContainer&&)      = default;

    CSVContainer& operator=(const CSVContainer&) = default;
    CSVContainer& operator=(CSVContainer&&) = default;

    ~CSVContainer() = default;

    CSVContainer(ValueType* T, const std::size_t rowDim, const std::size_t colDim);

    /* @brief Returns number of rows */
    std::size_t rowCount() const noexcept;

    /* @brief Returns number of columns */
    std::size_t columnCount() const noexcept;

//  typename std::vector<RowSPtr<ValueType>>::iterator begin();
//  typename std::vector<RowSPtr<ValueType>>::iterator end();
//  typename std::vector<RowSPtr<ValueType>>::const_iterator cbegin();
//  typename std::vector<RowSPtr<ValueType>>::const_iterator cend();
 
    ValueType* data()
    {
        std::shared_lock slck(m_mutex);
        return m_data.data();
    }

	NonOwningArray<ValueType> get(const std::size_t rowIdx)
	{
        std::shared_lock slck(m_mutex);
		return m_data.row(rowIdx);
	}

    void set(const std::size_t row, const std::size_t column, ValueType&& value)
    {
		std::lock_guard lg(m_mutex);
        m_data.set(row, column, std::forward<ValueType>(value));
    }

    ValueType at(const std::size_t row, const std::size_t col) const
    {
        std::shared_lock slck(m_mutex);
        assert(row < rowCount());
        assert(col < columnCount());
        return m_data.at(row, col);
    }

    void resize(const std::size_t rowDim, const std::size_t colDim = 0);

private:
    /* @brief Used to control concurrent access to this container by thread pool */
    mutable std::shared_mutex m_mutex;

    /* @brief Flattened 2d matrix */
    Flat2DMatrix<ValueType> m_data;
};

/**
 * @brief Construct csv container from a flat representation
 * @param aPtr Pointer to the 1d array
 * @param rowDim Number of rows
 * @param colDim Number of columns
 */
template <typename ValueType>
CSVContainer<ValueType>::CSVContainer(ValueType*        aPtr,
                                      const std::size_t rowDim,
                                      const std::size_t colDim)
{
    assert(aPtr != 0);
    assert(rowDim != 0);
    assert(colDim != 0);

    // Refactor to use index sets
    m_data.resize(rowDim, colDim);
    for (std::size_t row : range(rowDim))
    {
        for (std::size_t col : range(colDim))
        {
            m_data.set(row, col, *(aPtr + row * columnCount() + col));
        }
    }
}

template <typename ValueType>
std::size_t CSVContainer<ValueType>::rowCount() const noexcept
{
    std::shared_lock slck(m_mutex);
    return m_data.rowCount();
}

template <typename ValueType>
std::size_t CSVContainer<ValueType>::columnCount() const noexcept
{
    std::shared_lock slck(m_mutex);
    return m_data.columnCount();
}

template <typename ValueType>
void CSVContainer<ValueType>::resize(const std::size_t rowDim, const std::size_t colDim)
{
    std::lock_guard lg(m_mutex);
    assert(rowDim > 0);
    m_data.resize(rowDim, colDim);
}

// /*
//  * Members
//  */
// template <typename ValueType>
// typename std::vector<RowSPtr<ValueType>>::iterator CSVContainer<ValueType>::begin()
// {
//     std::shared_lock slck(m_mutex);
//     return m_data.begin();
// }
// 
// template <typename ValueType>
// typename std::vector<RowSPtr<ValueType>>::iterator CSVContainer<ValueType>::end()
// {
//     std::shared_lock slck(m_mutex);
//     return m_data.end();
// }
// 
// template <typename ValueType>
// typename std::vector<RowSPtr<ValueType>>::const_iterator CSVContainer<ValueType>::cbegin()
// {
//     std::shared_lock slck(m_mutex);
//     return m_data.cbegin();
// }
// 
// template <typename ValueType>
// typename std::vector<RowSPtr<ValueType>>::const_iterator CSVContainer<ValueType>::cend()
// {
//     std::shared_lock slck(m_mutex);
//     return m_data.cend();
// }
// 
// /*
//  * Free functions.
//  */
// template <typename ValueType>
// typename std::vector<RowSPtr<ValueType>>::iterator begin(CSVContainer<ValueType>& c)
// {
//     return c.begin();
// }
// 
// template <typename ValueType>
// typename std::vector<RowSPtr<ValueType>>::iterator end(CSVContainer<ValueType>& c)
// {
//     return c.end();
// }
// 
// template <typename ValueType>
// typename std::vector<RowSPtr<ValueType>>::const_iterator cbegin(CSVContainer<ValueType>& c)
// {
//     return c.cbegin();
// }
// 
// template <typename ValueType>
// typename std::vector<RowSPtr<ValueType>>::const_iterator cend(CSVContainer<ValueType>& c)
// {
//     return c.cend();
// }

template <typename ValueType>
using CSVContainerSPtr = std::shared_ptr<CSVContainer<ValueType>>;

} // namespace structures
