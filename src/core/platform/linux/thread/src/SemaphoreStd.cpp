#include "thread/Semaphore.h"
#include <chrono>

Semaphore::Semaphore(uint32_t initialCount)
    : m_count(initialCount)
{
}

void Semaphore::post()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    ++m_count;
    m_cv.notify_one();
}

void Semaphore::wait()
{
    std::unique_lock<std::mutex> lock(m_mutex);
    m_cv.wait(lock, [this] { return m_count > 0; });
    --m_count;
}

bool Semaphore::tryWait()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_count > 0)
    {
        --m_count;
        return true;
    }
    return false;
}

bool Semaphore::waitFor(uint32_t timeoutMs)
{
    std::unique_lock<std::mutex> lock(m_mutex);
    bool result = m_cv.wait_for(lock, std::chrono::milliseconds(timeoutMs),
                                 [this] { return m_count > 0; });
    if (result)
    {
        --m_count;
    }
    return result;
}
