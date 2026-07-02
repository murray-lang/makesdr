#pragma once

#include <mutex>
#include <condition_variable>
#include <cstdint>

/**
 * @brief Cross-platform semaphore abstraction using std::condition_variable.
 * Used for signaling between threads (e.g., GPIO interrupt to processing thread).
 */
class Semaphore
{
public:
    /**
     * @brief Constructor. Creates a semaphore.
     * @param initialCount Initial count (0 = not signaled, 1+ = signaled).
     */
    explicit Semaphore(uint32_t initialCount = 0);

    /**
     * @brief Destructor.
     */
    ~Semaphore() = default;

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

private:
    std::mutex m_mutex;
    std::condition_variable m_cv;
    uint32_t m_count;
};
