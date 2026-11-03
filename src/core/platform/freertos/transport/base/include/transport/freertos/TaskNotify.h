#pragma once

#include <cstdint>

#ifdef __cplusplus
extern "C" {
#endif
#include "FreeRTOS.h"
#include "task.h"
#ifdef __cplusplus
}
#endif

//*****************************************************************************
// The few FreeRTOS calls the transport needs, gathered here so that a port
// whose kernel differs changes only this file. (ESP-IDF, for example, would
// detect interrupt context with xPortInIsrContext().)
//
// Transport wakes use their own task-notification index, so they cannot
// collide with anything using the default index 0: LVGL's FreeRTOS layer, for
// one, uses xTaskNotifyGive()/ulTaskNotifyTake() on index 0. The kernel must
// therefore have at least two notification entries per task:
//     #define configTASK_NOTIFICATION_ARRAY_ENTRIES 2
//*****************************************************************************

#ifndef MAKESDR_TRANSPORT_NOTIFY_INDEX
#define MAKESDR_TRANSPORT_NOTIFY_INDEX 1
#endif

static_assert(MAKESDR_TRANSPORT_NOTIFY_INDEX < configTASK_NOTIFICATION_ARRAY_ENTRIES,
              "Transport needs its own task-notification index: "
              "raise configTASK_NOTIFICATION_ARRAY_ENTRIES");

namespace TaskNotify
{
  inline constexpr UBaseType_t Index = MAKESDR_TRANSPORT_NOTIFY_INDEX;

  // xPortIsInsideInterrupt() is provided by the Cortex-M ports (ARM_CM4F, ARM_CM7, ...).
  inline bool inInterrupt() { return xPortIsInsideInterrupt() == pdTRUE; }

  // Sets bits in task's transport notification value, from a task or an ISR.
  // From an ISR, a context switch to the woken task is requested for when the
  // ISR returns.
  inline void setBits(TaskHandle_t task, uint32_t bits)
  {
    if (inInterrupt()) {
      BaseType_t higherPriorityTaskWoken = pdFALSE;
      xTaskNotifyIndexedFromISR(task, Index, bits, eSetBits, &higherPriorityTaskWoken);
      portYIELD_FROM_ISR(higherPriorityTaskWoken);
    } else {
      xTaskNotifyIndexed(task, Index, bits, eSetBits);
    }
  }

  // ISR-only form for callers that wake several tasks and yield once.
  inline void setBitsFromISR(TaskHandle_t task, uint32_t bits, BaseType_t* higherPriorityTaskWoken)
  {
    xTaskNotifyIndexedFromISR(task, Index, bits, eSetBits, higherPriorityTaskWoken);
  }

  // Blocks the calling task until some bits are set or timeout expires, then
  // returns and clears all of them (0 on timeout).
  inline uint32_t wait(TickType_t timeout)
  {
    uint32_t bits = 0;
    xTaskNotifyWaitIndexed(Index, 0, UINT32_MAX, &bits, timeout);
    return bits;
  }
}
