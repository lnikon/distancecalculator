#ifndef L1METRIC_HPP
#define L1METRIC_HPP

#include "MetricBase.hpp"

#include <cmath>

template <typename ValueType, std::size_t N>
struct LNFunctor
{
    ValueType operator()(ValueType lhs, ValueType rhs)
    {
		return std::pow(lhs - rhs, N);
    }
};

template <typename ValueType>
using L1Metric = MetricBase<ValueType, LNFunctor<ValueType, 1>>;

template <typename ValueType>
using L2Metric = MetricBase<ValueType, LNFunctor<ValueType, 2>>;

#endif
