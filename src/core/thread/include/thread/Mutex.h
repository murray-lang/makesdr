#pragma once

#ifdef USE_FREERTOS
#include "FreeRTOS.h"
#include "semphr.h"
#else
#include <mutex>
#endif

/**
 * @brief Cross-platform mutex abstraction supporting std::mutex and FreeRTOS.
 */
class Mutex
{
public:
    /**
     * @brief Constructor. Creates a mutex.
     */
    Mutex();

    /**
     * @brief Destructor.
     */
    ~Mutex();

    // Delete copy operations
    Mutex(const Mutex&) = delete;
    Mutex& operator=(const Mutex&) = delete;

    // Delete move operations (mutexes should not be moved)
    Mutex(Mutex&&) = delete;
    Mutex& operator=(Mutex&&) = delete;

    /**
     * @brief Lock the mutex. Blocks if already locked.
     */
    void lock();

    /**
     * @brief Try to lock the mutex without blocking.
     * @return true if lock was acquired, false otherwise.
     */
    bool tryLock();

    /**
     * @brief Try to lock the mutex with a timeout.
     * @param timeoutMs Timeout in milliseconds.
     * @return true if lock was acquired, false if timeout occurred.
     */
    // bool tryLockFor(uint32_t timeoutMs);

    /**
     * @brief Unlock the mutex.
     */
    void unlock();

private:
#ifdef USE_FREERTOS
    SemaphoreHandle_t m_handle;
#if configSUPPORT_STATIC_ALLOCATION
    StaticSemaphore_t m_buffer;
#endif
#else
    std::mutex m_mutex;
#endif
};

/**
 * @brief RAII lock guard for Mutex.
 */
class LockGuard
{
public:
    /**
     * @brief Constructor. Locks the mutex.
     * @param mutex Reference to the mutex to lock.
     */
    explicit LockGuard(Mutex& mutex);

    /**
     * @brief Destructor. Unlocks the mutex.
     */
    ~LockGuard();

    // Delete copy and move operations
    LockGuard(const LockGuard&) = delete;
    LockGuard& operator=(const LockGuard&) = delete;
    LockGuard(LockGuard&&) = delete;
    LockGuard& operator=(LockGuard&&) = delete;

private:
    Mutex& m_mutex;
};
