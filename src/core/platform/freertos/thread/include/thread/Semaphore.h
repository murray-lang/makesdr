#pragma once

#include "FreeRTOS.h"
#include "semphr.h"
#include <cstdint>

/**
 * @brief Cross-platform semaphore abstraction using FreeRTOS binary semaphore.
 * Used for signaling between threads (e.g., GPIO interrupt to processing thread).
 */
class Semaphore
{
public:
    /**
     * @brief Constructor. Creates a binary semaphore.
     * @param initialCount Initial count (0 = not signaled, 1 = signaled).
     */
    explicit Semaphore(uint32_t initialCount = 0);

    /**
     * @brief Destructor.
     */
    ~Semaphore();

    // Delete copy operations
    Semaphore(const Semaphore&) = delete;
    Semaphore& operator=(const Semaphore&) = delete;

    // Delete move operations
    Semaphore(Semaphore&&) = delete;
    Semaphore& operator=(Semaphore&&) = delete;

    /**
     * @brief Signal the semaphore (increment/post/release).
     * Wakes up one waiting thread.
     */
    void post();

    /**
     * @brief Wait for the semaphore (decrement/acquire).
     * Blocks until semaphore is signaled.
     */
    void wait();

    /**
     * @brief Try to wait for the semaphore without blocking.
     * @return true if semaphore was acquired, false otherwise.
     */
    bool tryWait();

    /**
     * @brief Wait for the semaphore with a timeout.
     * @param timeoutMs Timeout in milliseconds.
     * @return true if semaphore was acquired, false if timeout occurred.
     */
    bool waitFor(uint32_t timeoutMs);

    /**
     * @brief Post from ISR context.
     * @param pxHigherPriorityTaskWoken Set to pdTRUE if context switch needed.
     */
    bool postFromISR(BaseType_t* pxHigherPriorityTaskWoken);

private:
    SemaphoreHandle_t m_handle;
#if configSUPPORT_STATIC_ALLOCATION
    StaticSemaphore_t m_buffer;
#endif
};
