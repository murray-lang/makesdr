#include "thread/Semaphore.h"

Semaphore::Semaphore(uint32_t initialCount)
    : m_handle(nullptr)
{
#if configSUPPORT_STATIC_ALLOCATION
    m_handle = xSemaphoreCreateBinaryStatic(&m_buffer);
#else
    m_handle = xSemaphoreCreateBinary();
#endif

    // If initial count is non-zero, give the semaphore
    if (initialCount > 0 && m_handle != nullptr)
    {
        xSemaphoreGive(m_handle);
    }
}

Semaphore::~Semaphore()
{
    if (m_handle != nullptr)
    {
        vSemaphoreDelete(m_handle);
    }
}

void Semaphore::post()
{
    if (m_handle != nullptr)
    {
        xSemaphoreGive(m_handle);
    }
}

void Semaphore::wait()
{
    if (m_handle != nullptr)
    {
        xSemaphoreTake(m_handle, portMAX_DELAY);
    }
}

bool Semaphore::tryWait()
{
    if (m_handle != nullptr)
    {
        return xSemaphoreTake(m_handle, 0) == pdTRUE;
    }
    return false;
}

bool Semaphore::waitFor(uint32_t timeoutMs)
{
    if (m_handle != nullptr)
    {
        TickType_t ticks = pdMS_TO_TICKS(timeoutMs);
        return xSemaphoreTake(m_handle, ticks) == pdTRUE;
    }
    return false;
}

bool Semaphore::postFromISR(BaseType_t* pxHigherPriorityTaskWoken)
{
    if (m_handle != nullptr)
    {
        return xSemaphoreGiveFromISR(m_handle, pxHigherPriorityTaskWoken) == pdPASS;
    }
  return false;
}
