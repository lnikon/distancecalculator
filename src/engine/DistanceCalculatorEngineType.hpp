#pragma once

enum class DistanceCalculatorEngineEngineKind {
  Sequential = 0,
  CPPThreads,
  CUDA,
  MPI
};

using EngineKind = DistanceCalculatorEngineEngineKind;

enum class MetricKind {
	L1Metric = 0,
	L2Metric,
	HammingDistance
};

