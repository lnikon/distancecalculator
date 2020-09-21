#pragma once

#include "CSVContainer.hpp"

#include <functional>
#include <iostream>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>

namespace threadpool
{

template <typename ValueType>
class Job
{
public:
    using CSVContainerSPtr = structures::CSVContainerSPtr<ValueType>;
    using RowSPtr          = structures::RowSPtr<ValueType>;
    using JobFn            = std::function<CSVContainerSPtr(const RowSPtr, const CSVContainerSPtr)>;

    Job(const RowSPtr query, const CSVContainerSPtr dataset)
        : m_query(query)
        , m_dataset(dataset)

    {
        m_fn = [this](const RowSPtr query, const CSVContainerSPtr dataset) {
            auto        result    = std::make_shared<structures::CSVContainer<ValueType>>();
            const auto& crDataset = *dataset;
            for (const auto datasetRow : *dataset)
            {
                // Using simple L1
                auto resultRow = std::make_shared<structures::Row<ValueType>>();
                std::transform(std::cbegin(*datasetRow),
                               std::cend(*datasetRow),
                               std::cbegin(*query),
                               std::back_inserter(*resultRow),
                               [](ValueType rhs, ValueType lhs) { return rhs - lhs; });
                result->append(resultRow);
            }
            return dataset;
        };
    }

    CSVContainerSPtr exec() const
    {
        return m_fn(m_query, m_dataset);
    }

private:
    const RowSPtr          m_query;
    const CSVContainerSPtr m_dataset;
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
        LockGuard<std::mutex> lg(m_mutex);
        return m_jobQueue.empty();
    }

    void push(std::shared_ptr<Job<ValueType>> job)
    {
        LockGuard<std::mutex> lg(m_mutex);
        m_jobQueue.push(job);
    }

    std::shared_ptr<Job<ValueType>> front()
    {
        LockGuard<std::mutex> lg(m_mutex);
        auto                  job = m_jobQueue.front();

        m_jobQueue.pop();
        return job;
    }

private:
    std::mutex                                  m_mutex;
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
    {
        auto workerLoop = [this]() {
            int count = 0;
            while (m_jobQueue->empty())
            {
            }

            std::shared_ptr<Job<ValueType>> job = m_jobQueue->front();
            job->exec();
        };

        // TODO: Switch to rangev3 library
        for (auto idx : range(0, m_hwdThreadCount))
        {
            m_workers.emplace_back(std::thread(workerLoop));
        }
    }

    ~ThreadPool()
    {
        for (auto& thread : m_workers)
        {
            if (thread.joinable())
            {
                thread.join();
            }
        }
    }

private:
    const unsigned                       m_hwdThreadCount = std::thread::hardware_concurrency();
    std::vector<std::thread>             m_workers;
    std::vector<ThreadGuard>             m_workerGuards;
    std::shared_ptr<JobQueue<ValueType>> m_jobQueue;
};

} // namespace threadpool
