#define CATCH_CONFIG_MAIN

#include "ThreadPool.hpp"

#include <catch2/catch.hpp>

#include <memory>

TEST_CASE("ThreadPool: Execute one job", "[threadpool]")
{
    auto jobs = std::make_shared<threadpool::JobQueue<float>>();
    auto pool = threadpool::ThreadPool<float>(jobs);

    for (int i : threadpool::range(0, 100))
    {
        auto container = std::make_shared<structures::CSVContainer<float>>();
        auto row       = std::make_shared<structures::Row<float>>();
        auto job       = std::make_shared<threadpool::Job<float>>(row, container);
        jobs->push(job);
    }

    while (true)
    {
    }
}
