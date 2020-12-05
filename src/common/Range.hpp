#ifndef RANGE_HPP
#define RANGE_HPP

#include <algorithm>
#include <cassert>
#include <vector>

template <typename IndexType>
std::vector<IndexType> range(IndexType start, IndexType end, IndexType step)
{
    assert(((void)"end < start", end >= start));

    IndexType current{};
    auto      gen = [&current]() { return current++; };

    const auto             count{end - start};
    std::vector<IndexType> indices(count);
    std::generate_n(indices.begin(), count, gen);

    return indices;
}

template <typename IndexType>
std::vector<IndexType> range(IndexType count)
{
    return range(IndexType{0}, count, IndexType{1});
}

#endif
