#ifndef HAMMING_DISTANCE
#define HAMMING_DISTANCE

#include "MetricBase.hpp"

template <typename ValueType>
struct HammingDistanceFunctor
{
    bool operator()(ValueType lhs, ValueType rhs)
    {
		return lhs == rhs;
    }
};

template <typename ValueType>
using HammingDistance = MetricBase<ValueType, HammingDistanceFunctor<ValueType>>;

#endif
