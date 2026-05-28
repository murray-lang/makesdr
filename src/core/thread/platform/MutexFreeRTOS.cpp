#include "../include/thread/Mutex.h"

Mutex::Mutex()
{
#if configSUPPORT_STATIC_ALLOCATION
    m_handle = xSemaphoreCreateMutexStatic(&m_buffer);
#else
    m_handle = xSemaphoreCreateMutex();
#endif
    configASSERT(m_handle != nullptr);
}

Mutex::~Mutex()
{
    if (m_handle != nullptr)
    {
        vSemaphoreDelete(m_handle);
    }
}

void Mutex::lock()
{
    xSemaphoreTake(m_handle, portMAX_DELAY);
}

bool Mutex::tryLock()
{
    return xSemaphoreTake(m_handle, 0) == pdTRUE;
}

// bool Mutex::tryLockFor(uint32_t timeoutMs)
// {
//     return xSemaphoreTake(m_handle, pdMS_TO_TICKS(timeoutMs)) == pdTRUE;
// }

void Mutex::unlock()
{
    xSemaphoreGive(m_handle);
}

// LockGuard implementation
LockGuard::LockGuard(Mutex& mutex)
    : m_mutex(mutex)
{
    m_mutex.lock();
}

LockGuard::~LockGuard()
{
    m_mutex.unlock();
}
