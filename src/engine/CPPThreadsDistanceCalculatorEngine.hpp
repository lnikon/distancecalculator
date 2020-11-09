#pragma once

#include "IDistanceCalculatorEngine.hpp"
#include "ThreadPool.hpp"

template <typename ValueType>
class CPPThreadsDistanceCalculatorEngine
    : public IDistanceCalculatorEngine<ValueType> {
public:
  structures::CSVContainerSPtr<ValueType>
  calculate(structures::CSVContainerSPtr<ValueType> query,
            structures::CSVContainerSPtr<ValueType>
                dataset) /* noexcept */ const override {
    auto result = std::make_shared<structures::CSVContainer<float>>();
    const std::size_t distanceMatrixRowCnt =
        query->rowCount() * dataset->rowCount();
    result->resize(distanceMatrixRowCnt);

    auto jobs = std::make_shared<threadpool::JobQueue<float>>();
    std::size_t rowIdx = 0;
    for (auto row : *query) {
      const std::size_t beginIdx = rowIdx * dataset->rowCount();
      auto job = std::make_shared<threadpool::Job<float>>(row, beginIdx,
                                                          dataset, result);

      jobs->push(job);
      rowIdx++;
    }

    auto pool = threadpool::ThreadPool<float>(jobs);
    while (!pool.done()) {
    }

    return result;
  }

  DistanceCalculatorEngineType type() /* noexcept */ const override {
    return DistanceCalculatorEngineType::CPPThreads;
  }
};
