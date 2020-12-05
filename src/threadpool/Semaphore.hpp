#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include <condition_variable>
#include <mutex>

class Semaphore
{
public:
    explicit Semaphore(const std::size_t count)
        : m_count(count)
    {
    }

    void Wait()
    {
        std::unique_lock<std::mutex> ulock(m_mutex);
        m_cv.wait(ulock, [this] { return m_count; });
        m_count--;
    }

    void Signal()
    {
        std::unique_lock<std::mutex> ulock(m_mutex);
        m_count++;
        m_cv.notify_one();
    }

private:
    std::size_t             m_count;
    std::mutex              m_mutex;
    std::condition_variable m_cv;
};

#endif
