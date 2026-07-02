#ifndef FREERTOS_THREAD_H_
#define FREERTOS_THREAD_H_

#include "Runnable.h"
#include <cstdint>

#ifdef __cplusplus
extern "C" {
#endif

#include "FreeRTOS.h"
#include "task.h"
#ifdef __cplusplus
}
#endif


/**
 * @brief Cross-platform thread abstraction supporting std::thread and FreeRTOS.
 * Similar to Java's Thread class with a Runnable interface.
 */
class Thread
{
public:
  /**
   * @brief Default constructor. Creates a thread without a Runnable.
   */
  Thread();

  /**
   * @brief Constructor with Runnable.
   * @param runnable Reference to a Runnable object whose run() method will be executed.
   */
  explicit Thread(Runnable& runnable);

  /**
   * @brief Destructor. Stops the thread if still running.
   */
  ~Thread();

  // Delete copy operations
  Thread(const Thread&) = delete;
  Thread& operator=(const Thread&) = delete;

  // Move operations
  Thread(Thread&& other) noexcept;
  Thread& operator=(Thread&& other) noexcept;

  /**
   * @brief Set the Runnable to execute.
   * @param runnable Reference to a Runnable object.
   */
  void setRunnable(Runnable& runnable);

  /**
   * @brief Start the thread execution.
   * @param stackSize Stack size for the thread (FreeRTOS only, ignored for std::thread).
   * @param priority Thread priority (FreeRTOS only, ignored for std::thread).
   * @param name Optional thread name.
   * @return true if thread started successfully, false otherwise.
   */
  bool start(uint32_t stackSize = 2048, uint32_t priority = 1, const char* name = "Thread");

  /**
   * @brief Request the thread to stop and wait for it to finish.
   * Sets the stop flag and joins/waits for the thread.
   */
  void stop();

  /**
   * @brief Check if a stop has been requested.
   * @return true if stop() has been called.
   */
  bool shouldStop() const;

  /**
   * @brief Check if the thread is currently running.
   * @return true if the thread is running.
   */
  bool isRunning() const;

  /**
   * @brief Wait for the thread to finish execution.
   */
  void join();

  /**
   * @brief Detach the thread (platform-dependent behavior).
   */
  void detach();

  /**
   * @brief Sleep for a specified number of milliseconds.
   * @param milliseconds Duration to sleep.
   */
  static void sleep(uint32_t milliseconds);

  /**
   * @brief Yield execution to other threads.
   */
  static void yield();

private:
  /**
   * @brief Internal thread function that wraps the Runnable.
   */
  void threadFunction();

  static void taskFunction(void* param);

private:
  Runnable* m_runnable;
  TaskHandle_t m_taskHandle;
  volatile bool m_stopRequested;
  volatile bool m_isRunning;
};

#endif //FREERTOS_THREAD_H_
