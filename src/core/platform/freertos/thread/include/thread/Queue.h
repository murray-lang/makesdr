#pragma once

#include "FreeRTOS.h"
#include "queue.h"
#include <cstdint>

/**
 * @brief Cross-platform queue abstraction using FreeRTOS queue.
 * Used for passing data between threads or from ISR to thread.
 *
 * @tparam T The type of elements stored in the queue.
 * @tparam MaxSize Maximum number of elements the queue can hold.
 */
template<typename T, size_t MaxSize>
class Queue
{
public:
    /**
     * @brief Constructor. Creates a FreeRTOS queue.
     */
    Queue();

    /**
     * @brief Destructor.
     */
    ~Queue();

    // Delete copy operations
    Queue(const Queue&) = delete;
    Queue& operator=(const Queue&) = delete;

    // Delete move operations
    Queue(Queue&&) = delete;
    Queue& operator=(Queue&&) = delete;

    /**
     * @brief Send an item to the queue (blocking).
     * @param item The item to send.
     * @param timeoutMs Timeout in milliseconds (default: wait forever).
     * @return true if item was sent successfully, false on timeout.
     */
    bool send(const T& item, uint32_t timeoutMs = portMAX_DELAY);

    /**
     * @brief Send an item to the queue from ISR context.
     * @param item The item to send.
     * @param pxHigherPriorityTaskWoken Set to pdTRUE if context switch needed.
     * @return true if item was sent successfully, false if queue was full.
     */
    bool sendFromISR(const T& item, BaseType_t* pxHigherPriorityTaskWoken);

    /**
     * @brief Receive an item from the queue (blocking).
     * @param item Reference to store the received item.
     * @param timeoutMs Timeout in milliseconds (default: wait forever).
     * @return true if item was received successfully, false on timeout.
     */
    bool receive(T& item, uint32_t timeoutMs = portMAX_DELAY);

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
    QueueHandle_t m_handle;
#if configSUPPORT_STATIC_ALLOCATION
    StaticQueue_t m_queueBuffer;
    uint8_t m_storageBuffer[MaxSize * sizeof(T)];
#endif
};

// Template implementation
template<typename T, size_t MaxSize>
Queue<T, MaxSize>::Queue()
    : m_handle(nullptr)
{
#if configSUPPORT_STATIC_ALLOCATION
    m_handle = xQueueCreateStatic(MaxSize, sizeof(T), m_storageBuffer, &m_queueBuffer);
#else
    m_handle = xQueueCreate(MaxSize, sizeof(T));
#endif
    configASSERT(m_handle != nullptr);
}

template<typename T, size_t MaxSize>
Queue<T, MaxSize>::~Queue()
{
    if (m_handle != nullptr)
    {
        vQueueDelete(m_handle);
    }
}

template<typename T, size_t MaxSize>
bool Queue<T, MaxSize>::send(const T& item, uint32_t timeoutMs)
{
    if (m_handle == nullptr)
    {
        return false;
    }

    TickType_t ticks = (timeoutMs == portMAX_DELAY) ? portMAX_DELAY : pdMS_TO_TICKS(timeoutMs);
    return xQueueSend(m_handle, &item, ticks) == pdTRUE;
}

template<typename T, size_t MaxSize>
bool Queue<T, MaxSize>::sendFromISR(const T& item, BaseType_t* pxHigherPriorityTaskWoken)
{
    if (m_handle == nullptr)
    {
        return false;
    }

    return xQueueSendFromISR(m_handle, &item, pxHigherPriorityTaskWoken) == pdTRUE;
}

template<typename T, size_t MaxSize>
bool Queue<T, MaxSize>::receive(T& item, uint32_t timeoutMs)
{
    if (m_handle == nullptr)
    {
        return false;
    }

    TickType_t ticks = (timeoutMs == portMAX_DELAY) ? portMAX_DELAY : pdMS_TO_TICKS(timeoutMs);
    return xQueueReceive(m_handle, &item, ticks) == pdTRUE;
}

template<typename T, size_t MaxSize>
bool Queue<T, MaxSize>::tryReceive(T& item)
{
    if (m_handle == nullptr)
    {
        return false;
    }

    return xQueueReceive(m_handle, &item, 0) == pdTRUE;
}

template<typename T, size_t MaxSize>
bool Queue<T, MaxSize>::isEmpty() const
{
    if (m_handle == nullptr)
    {
        return true;
    }

    return uxQueueMessagesWaiting(m_handle) == 0;
}

template<typename T, size_t MaxSize>
bool Queue<T, MaxSize>::isFull() const
{
    if (m_handle == nullptr)
    {
        return false;
    }

    return uxQueueSpacesAvailable(m_handle) == 0;
}

template<typename T, size_t MaxSize>
size_t Queue<T, MaxSize>::size() const
{
    if (m_handle == nullptr)
    {
        return 0;
    }

    return static_cast<size_t>(uxQueueMessagesWaiting(m_handle));
}
