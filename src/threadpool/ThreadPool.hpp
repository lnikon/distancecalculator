#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <algorithm>
#include <atomic>
#include <cassert>
#include <condition_variable>
#include <functional>
#include <future>
#include <iostream>
#include <iterator>
#include <memory>
#include <mutex>
#include <queue>
#include <ranges>
#include <thread>
#include <unordered_map>
#include <vector>

#include "../common/Range.hpp"
#include "Semaphore.hpp"

class ThreadPool : public std::enable_shared_from_this<ThreadPool>
{
public:
    explicit ThreadPool(std::size_t num_threads = std::thread::hardware_concurrency())
        : m_num_threads(num_threads)
        , m_threads_sem(0)
        , m_tasks_sem(1)
        , m_terminate(false)
    {
        /* Handle the case when 0 passed as a @num_threads */
        num_threads = std::max(std::size_t(1), num_threads);
        for (; num_threads != 0; num_threads--)
        {
            m_threads.emplace_back(std::thread([this] { this->Task(this); }));
        }
    }

	~ThreadPool()
	{
		m_terminate = true;
		for (auto _ : range(m_num_threads))
		{
			(void)_;
			m_threads_sem.Signal();
		}

		for (auto& thread : m_threads)
		{
			thread.join();
		}
	}

	template <typename F, typename... Args>
		auto Submit(F&& f, Args&&... args) -> std::future<typename std::result_of<F(Args...)>::type>
		{
			auto task =
				std::make_shared<std::packaged_task<typename std::result_of<F(Args...)>::type()>>(
						std::bind(std::forward<F>(f), std::forward<Args>(args)...));

			auto task_result  = task->get_future();
			auto task_wrapper = [task] { (*task)(); };

			m_tasks_sem.Wait();
			m_tasks.emplace(task_wrapper);
			m_tasks_sem.Signal();

			m_threads_sem.Signal();
			return task_result;
		}

private:
	void Task(ThreadPool* thread_pool)
	{
		while (true)
		{
			thread_pool->m_threads_sem.Wait();

			if (thread_pool->m_terminate)
			{
				break;
			}

			thread_pool->m_tasks_sem.Wait();
			auto task = std::move(thread_pool->m_tasks.front());
			thread_pool->m_tasks.pop();
			thread_pool->m_tasks_sem.Signal();

			if (thread_pool->m_terminate)
			{
				break;
			}

			task();
		}
	}

private:
	const std::size_t m_num_threads;

	/* Thread Handling */
	std::vector<std::thread> m_threads;
	Semaphore                m_threads_sem;

	/* Task Handling */
	std::queue<std::function<void()>> m_tasks;
	Semaphore                         m_tasks_sem;

	/* To control task termination */
	bool m_terminate;
};

#endif
