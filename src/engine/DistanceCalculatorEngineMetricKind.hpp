#ifndef METRIC_KIND_HPP
#define METRIC_KIND_HPP

enum class DistanceCalculatorMetricKind
{
    L1Metric = 0,
    L2Metric,
    HammingDistance
};

using MetricKind = DistanceCalculatorMetricKind;

#endif
