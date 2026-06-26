#include "../include/thread/Thread.h"
#include <chrono>
#include <thread>

Thread::Thread()
    : m_runnable(nullptr)
    , m_stopRequested(false)
    , m_isRunning(false)
{
}

Thread::Thread(Runnable& runnable)
    : m_runnable(&runnable)
    , m_stopRequested(false)
    , m_isRunning(false)
{
}

Thread::~Thread()
{
    if (isRunning())
    {
        stop();
    }

    if (m_thread.joinable())
    {
        m_thread.join();
    }
}

Thread::Thread(Thread&& other) noexcept
    : m_runnable(other.m_runnable)
    , m_thread(std::move(other.m_thread))
    , m_stopRequested(other.m_stopRequested.load())
    , m_isRunning(other.m_isRunning.load())
{
    other.m_runnable = nullptr;
    other.m_stopRequested = false;
    other.m_isRunning = false;
}

Thread& Thread::operator=(Thread&& other) noexcept
{
    if (this != &other)
    {
        // Stop current thread if running
        if (isRunning())
        {
            stop();
        }

        if (m_thread.joinable())
        {
            m_thread.join();
        }

        m_runnable = other.m_runnable;
        m_thread = std::move(other.m_thread);
        m_stopRequested.store(other.m_stopRequested.load());
        m_isRunning.store(other.m_isRunning.load());

        other.m_runnable = nullptr;
        other.m_stopRequested = false;
        other.m_isRunning = false;
    }
    return *this;
}

void Thread::setRunnable(Runnable& runnable)
{
    m_runnable = &runnable;
}

bool Thread::start(uint32_t stackSize, uint32_t priority, const char* name)
{
    if (m_runnable == nullptr)
    {
        return false;
    }

    if (isRunning())
    {
        return false;
    }

    m_stopRequested = false;

    (void)stackSize;  // Unused for std::thread
    (void)priority;   // Unused for std::thread
    (void)name;       // Could use pthread_setname_np on Linux, but keeping simple

    try
    {
        m_thread = std::thread(&Thread::threadFunction, this);
        m_isRunning = true;
        return true;
    }
    catch (...)
    {
        return false;
    }
}

void Thread::stop()
{
    m_stopRequested = true;
    join();
}

bool Thread::shouldStop() const
{
    return m_stopRequested;
}

bool Thread::isRunning() const
{
    return m_isRunning;
}

void Thread::join()
{
    if (m_thread.joinable())
    {
        m_thread.join();
    }
}

void Thread::detach()
{
    if (m_thread.joinable())
    {
        m_thread.detach();
    }
}

void Thread::sleep(uint32_t milliseconds)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

void Thread::yield()
{
    std::this_thread::yield();
}

void Thread::threadFunction()
{
    if (m_runnable != nullptr)
    {
        m_runnable->run();
    }
    m_isRunning = false;
}
