#pragma once

#include <mutex>
#include <condition_variable>
#include <queue>
#include <cstdint>

/**
 * @brief Cross-platform queue abstraction using std::queue.
 * Used for passing data between threads.
 *
 * @tparam T The type of elements stored in the queue.
 * @tparam MaxSize Maximum number of elements the queue can hold.
 */
template<typename T, size_t MaxSize>
class Queue
{
public:
    /**
     * @brief Constructor. Creates a queue.
     */
    Queue() = default;

    /**
     * @brief Destructor.
     */
    ~Queue() = default;

    // Delete copy operations
    Queue(const Queue&) = delete;
    Queue& operator=(const Queue&) = delete;

    // Delete move operations
    Queue(Queue&&) = delete;
    Queue& operator=(Queue&&) = delete;

    /**
     * @brief Send an item to the queue (blocking).
     * @param item The item to send.
     * @param timeoutMs Timeout in milliseconds (0 = no timeout, wait forever).
     * @return true if item was sent successfully, false on timeout.
     */
    bool send(const T& item, uint32_t timeoutMs = 0);

    /**
     * @brief Send an item to the queue from ISR context.
     * Note: On Linux, this is the same as send() with no timeout.
     * @param item The item to send.
     * @param pxHigherPriorityTaskWoken Ignored on Linux (for API compatibility).
     * @return true if item was sent successfully, false if queue was full.
     */
    bool sendFromISR(const T& item, void* pxHigherPriorityTaskWoken = nullptr);

    /**
     * @brief Receive an item from the queue (blocking).
     * @param item Reference to store the received item.
     * @param timeoutMs Timeout in milliseconds (0 = wait forever).
     * @return true if item was received successfully, false on timeout.
     */
    bool receive(T& item, uint32_t timeoutMs = 0);

    /**
     * @brief Try to receive an item without blocking.
     * @param item Reference to store the received item.
     * @return true if item was received, false if queue was empty.
     */
    bool tryReceive(T& item);

    /**
     * @brief Check if the queue is empty.
     * @return true if queue is empty, false otherwise.
     */
    bool isEmpty() const;

    /**
     * @brief Check if the queue is full.
     * @return true if queue is full, false otherwise.
     */
    bool isFull() const;

    /**
     * @brief Get the number of items currently in the queue.
     * @return Number of items in the queue.
     */
    size_t size() const;

private:
    mutable std::mutex m_mutex;
    std::condition_variable m_cv;
    std::queue<T> m_queue;
};

// Template implementation
template<typename T, size_t MaxSize>
bool Queue<T, MaxSize>::send(const T& item, uint32_t timeoutMs)
{
    std::unique_lock<std::mutex> lock(m_mutex);

    // Wait if queue is full
    if (timeoutMs == 0)
    {
        // Wait forever
        m_cv.wait(lock, [this] { return m_queue.size() < MaxSize; });
    }
    else
    {
        // Wait with timeout
        if (!m_cv.wait_for(lock, std::chrono::milliseconds(timeoutMs),
                           [this] { return m_queue.size() < MaxSize; }))
        {
            return false; // Timeout
        }
    }

    m_queue.push(item);
    m_cv.notify_one();
    return true;
}

template<typename T, size_t MaxSize>
bool Queue<T, MaxSize>::sendFromISR(const T& item, void* pxHigherPriorityTaskWoken)
{
    // On Linux, ISR context doesn't exist, so just try to send without blocking
    std::unique_lock<std::mutex> lock(m_mutex);

    if (m_queue.size() >= MaxSize)
    {
        return false; // Queue full
    }

    m_queue.push(item);
    m_cv.notify_one();
    return true;
}

template<typename T, size_t MaxSize>
bool Queue<T, MaxSize>::receive(T& item, uint32_t timeoutMs)
{
    std::unique_lock<std::mutex> lock(m_mutex);

    // Wait if queue is empty
    if (timeoutMs == 0)
    {
        // Wait forever
        m_cv.wait(lock, [this] { return !m_queue.empty(); });
    }
    else
    {
        // Wait with timeout
        if (!m_cv.wait_for(lock, std::chrono::milliseconds(timeoutMs),
                           [this] { return !m_queue.empty(); }))
        {
            return false; // Timeout
        }
    }

    item = m_queue.front();
    m_queue.pop();
    m_cv.notify_one(); // Notify waiting senders
    return true;
}

template<typename T, size_t MaxSize>
bool Queue<T, MaxSize>::tryReceive(T& item)
{
    std::unique_lock<std::mutex> lock(m_mutex);

    if (m_queue.empty())
    {
        return false;
    }

    item = m_queue.front();
    m_queue.pop();
    m_cv.notify_one();
    return true;
}

template<typename T, size_t MaxSize>
bool Queue<T, MaxSize>::isEmpty() const
{
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_queue.empty();
}

template<typename T, size_t MaxSize>
bool Queue<T, MaxSize>::isFull() const
{
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_queue.size() >= MaxSize;
}

template<typename T, size_t MaxSize>
size_t Queue<T, MaxSize>::size() const
{
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_queue.size();
}
