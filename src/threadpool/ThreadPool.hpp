#pragma once

#include "CSVContainer.hpp"

#include <functional>
#include <iostream>
#include <memory>
#include <mutex>
#include <queue>
#include <shared_mutex>
#include <thread>

namespace threadpool
{

template <typename ValueType>
class Job
{
public:
    using CSVContainerSPtr = structures::CSVContainerSPtr<ValueType>;
    using RowSPtr          = structures::RowSPtr<ValueType>;
    using JobFn            = std::function<CSVContainerSPtr(
        const RowSPtr, const std::size_t, const CSVContainerSPtr, CSVContainerSPtr)>;

    Job(const RowSPtr          query,
        const std::size_t      queryIdx,
        const CSVContainerSPtr dataset,
        CSVContainerSPtr       result)
        : m_query(query)
        , m_queryIdx(queryIdx)
        , m_dataset(dataset)
        , m_result(result)
    {
        m_fn = [this](const RowSPtr          query,
                      const std::size_t      queryIdx,
                      const CSVContainerSPtr dataset,
                      CSVContainerSPtr       result) {
            const auto& crDataset     = *dataset;
            std::size_t datasetRowIdx = 0;
            for (const auto datasetRow : *dataset)
            {
                // Using simple L1
                auto resultRow = std::make_shared<structures::Row<ValueType>>();
                std::transform(std::cbegin(*datasetRow),
                               std::cend(*datasetRow),
                               std::cbegin(*query),
                               std::back_inserter(*resultRow),
                               [](ValueType rhs, ValueType lhs) { return rhs - lhs; });
                assert(resultRow != nullptr);
                result->set(queryIdx + datasetRowIdx, resultRow);
                datasetRowIdx++;
            }
            return result;
        };
    }

    CSVContainerSPtr exec() const
    {
        return m_fn(m_query, m_queryIdx, m_dataset, m_result);
    }

private:
    const RowSPtr          m_query;
    const std::size_t      m_queryIdx;
    const CSVContainerSPtr m_dataset;
    CSVContainerSPtr       m_result;
    JobFn                  m_fn;
};

template <typename Mutex>
using LockGuard = std::lock_guard<Mutex>;

template <typename ValueType>
class JobQueue
{
public:
    bool empty()
    {
        std::lock_guard<std::mutex> lg(m_mutex);
        return m_jobQueue.empty();
    }

    void push(std::shared_ptr<Job<ValueType>> job)
    {
        std::lock_guard<std::mutex> lg(m_mutex);
        m_jobQueue.push(job);
        m_cv.notify_one();
    }

    void front(std::shared_ptr<Job<ValueType>>& result)
    {
        std::unique_lock<std::mutex> lk(m_mutex);
        m_cv.wait(lk, [this] { return !m_jobQueue.empty(); });
        result = std::move(m_jobQueue.front());
        m_jobQueue.pop();
    }

private:
    std::mutex                                  m_mutex;
    std::condition_variable                     m_cv;
    std::queue<std::shared_ptr<Job<ValueType>>> m_jobQueue;
};

class ThreadGuard
{
public:
    explicit ThreadGuard(std::thread& thread)
        : m_thread(thread)
    {
    }

    ThreadGuard(const ThreadGuard&) = delete;
    ThreadGuard(ThreadGuard&&)      = delete;
    ThreadGuard& operator=(const ThreadGuard&) = delete;
    ThreadGuard& operator=(ThreadGuard&&) = delete;

    ~ThreadGuard()
    {
        if (m_thread.joinable())
        {
            m_thread.join();
        }
    }

private:
    std::thread& m_thread;
};

std::vector<std::size_t> range(std::size_t start, const std::size_t end)
{
    if (start >= end)
    {
        return std::vector<std::size_t>();
    }

    std::vector<std::size_t> indices;
    indices.reserve(end - start);
    while (start != end)
    {
        indices.push_back(start++);
    }

    return indices;
}

template <typename ValueType>
class ThreadPool
{
public:
    ThreadPool(std::shared_ptr<JobQueue<ValueType>> jobQueue)
        : m_jobQueue(jobQueue)
        , m_done(false)
    {
        auto workerLoop = [this]() {
            while (true)
            {
                while (m_jobQueue->empty())
                {
                    return;
                }

                std::shared_ptr<Job<ValueType>> job;
                m_jobQueue->front(job);
                job->exec();
            }
        };

        for (auto idx : range(0, m_hwdThreadCount))
        {
            m_workers.emplace_back(std::thread(workerLoop));
        }
    }

    bool done()
    {
        if (m_done)
        {
            return true;
        }

        for (auto& thread : m_workers)
        {
            if (thread.joinable())
            {
                thread.join();
            }
        }

        m_done = true;
        return m_done;
    }

    ~ThreadPool()
    {
        done();
    }

private:
    const unsigned                       m_hwdThreadCount = std::thread::hardware_concurrency();
    std::vector<std::thread>             m_workers;
    std::vector<ThreadGuard>             m_workerGuards;
    std::shared_ptr<JobQueue<ValueType>> m_jobQueue;
    bool                                 m_done;
};

} // namespace threadpool
