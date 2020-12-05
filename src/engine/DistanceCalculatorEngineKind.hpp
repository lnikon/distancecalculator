#ifndef ENGINE_KIND_HPP
#define ENGINE_KIND_HPP

enum class DistanceCalculatorEngineKind {
  Sequential = 0,
  CPPThreads,
  CUDA,
  MPI
};

using EngineKind = DistanceCalculatorEngineKind;

#endif
