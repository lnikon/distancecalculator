#pragma once

#include "IDistanceCalculatorEngine.hpp"
#include "ThreadPool.hpp"

template <typename ValueType>
class CPPThreadsDistanceCalculatorEngine : public IDistanceCalculatorEngine<ValueType>
{
public:
    structures::CSVContainerSPtr<ValueType> calculate(structures::CSVContainerSPtr<ValueType> query, structures::CSVContainerSPtr<ValueType> dataset) /* noexcept */ const override
    {
        auto result = std::make_shared<structures::CSVContainer<float>>();
        result->resize(query->rowCount() * dataset->rowCount());

        auto jobs = std::make_shared<threadpool::JobQueue<float>>();
        std::size_t rowIdx = 0;
        for (auto row : *query)
        {
            auto job       = std::make_shared<threadpool::Job<float>>(row, rowIdx, dataset, result);
            rowIdx++;
            jobs->push(job);
        }

        auto pool = threadpool::ThreadPool<float>(jobs);
        while (!jobs->empty()) { }

        return result;
    }

    DistanceCalculatorEngineType type() /* noexcept */ const override
    {
        return DistanceCalculatorEngineType::CPPThreads;
    }
};
