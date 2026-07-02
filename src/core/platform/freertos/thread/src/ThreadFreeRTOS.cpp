#include "thread/Thread.h"
#include "FreeRTOS.h"
#include "task.h"

Thread::Thread()
    : m_runnable(nullptr)
    , m_taskHandle(nullptr)
    , m_stopRequested(false)
    , m_isRunning(false)
{
}

Thread::Thread(Runnable& runnable)
    : m_runnable(&runnable)
    , m_taskHandle(nullptr)
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
}

Thread::Thread(Thread&& other) noexcept
    : m_runnable(other.m_runnable)
    , m_taskHandle(other.m_taskHandle)
    , m_stopRequested(other.m_stopRequested)
    , m_isRunning(other.m_isRunning)
{
    other.m_runnable = nullptr;
    other.m_taskHandle = nullptr;
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

        m_runnable = other.m_runnable;
        m_taskHandle = other.m_taskHandle;
        m_stopRequested = other.m_stopRequested;
        m_isRunning = other.m_isRunning;

        other.m_runnable = nullptr;
        other.m_taskHandle = nullptr;
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

    BaseType_t result = xTaskCreate(
        taskFunction,
        name,
        stackSize,
        this,
        priority,
        &m_taskHandle
    );

    if (result == pdPASS)
    {
        m_isRunning = true;
        return true;
    }
    return false;
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
    // FreeRTOS doesn't have a built-in join mechanism
    // Wait for the task to finish by polling
    while (m_isRunning)
    {
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

void Thread::detach()
{
    // FreeRTOS tasks are inherently detached
    m_taskHandle = nullptr;
}

void Thread::sleep(uint32_t milliseconds)
{
    vTaskDelay(pdMS_TO_TICKS(milliseconds));
}

void Thread::yield()
{
    taskYIELD();
}

void Thread::threadFunction()
{
    if (m_runnable != nullptr)
    {
        m_runnable->run();
    }
    m_isRunning = false;
}

void Thread::taskFunction(void* param)
{
    auto* thread = static_cast<Thread*>(param);
    thread->threadFunction();
    vTaskDelete(nullptr);  // Delete self when done
}
