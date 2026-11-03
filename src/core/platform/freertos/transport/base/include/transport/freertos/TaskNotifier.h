#pragma once

#include <atomic>

#include <settings/model/message/PayloadType.h>
#include <transport/PayloadTypeBits.h>

#include "TaskNotify.h"

//*****************************************************************************
// Wakes the consumer of a ring by setting its payload type's bit in the
// consumer task's transport notification value: the FreeRTOS counterpart of
// QtNotifier, for producer and consumer in the same program image (tasks on
// one core, or on an SMP part such as the ESP32).
//
// The producer may commit from a task or an ISR; the right notify call is
// chosen automatically.
//
// The consumer attaches a TaskRecipientT (anything with taskHandle()) when
// ready and detaches when going away. While nothing is attached,
// notifications are dropped and messages wait in the ring.
//
// detach() guarantees that no notification is started after it returns. One
// that a producer had already started may still arrive, setting a bit that
// the task's TaskDispatcher offers to whichever recipients remain. So the
// consumer task itself must outlive the last notification that could target
// it; transport tasks normally run for the life of the system.
//*****************************************************************************
class TaskNotifier
{
public:
  template<typename TargetT>
  void attach(TargetT* target)
  {
    const TaskHandle_t task = target->taskHandle();
    configASSERT(task != nullptr);  // TaskDispatcher not bound before attaching
    m_task.store(task, std::memory_order_release);
  }

  void detach() { m_task.store(nullptr, std::memory_order_release); }

  void operator()(PayloadType payloadType)
  {
    const TaskHandle_t task = m_task.load(std::memory_order_acquire);
    if (task != nullptr) {
      TaskNotify::setBits(task, PayloadTypeBits::maskOf(payloadType));
    }
  }

private:
  std::atomic<TaskHandle_t> m_task{nullptr};
};
