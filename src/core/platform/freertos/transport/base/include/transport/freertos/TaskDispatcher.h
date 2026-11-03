#pragma once

#include <cstddef>
#include <cstdint>

#include <settings/model/message/PayloadType.h>
#include <transport/PayloadTypeBits.h>

#include "TaskNotify.h"

#ifndef MAKESDR_TASK_DISPATCHER_MAX_RECIPIENTS
#define MAKESDR_TASK_DISPATCHER_MAX_RECIPIENTS 8
#endif

// Anything a TaskDispatcher can hand a payload type to: in practice a
// TaskRecipientT, which passes it on to its side's dispatch().
class IPayloadDispatch
{
public:
  virtual void dispatch(PayloadType payloadType) = 0;

protected:
  ~IPayloadDispatch() = default;
};

//*****************************************************************************
// The consumer task's end of FreeRTOS transport wakes: the counterpart of the
// Qt event loop that delivers MessageEvents to QtRecipientT objects.
//
// One per consuming task. Notifiers wake the task by setting the bit of the
// payload type that is waiting (see PayloadTypeBits); the task calls wait()
// and passes the bits to dispatch(), or calls waitAndDispatch(). Each set bit
// is offered to every registered recipient; a side ignores payload types that
// are not its own, as with Qt.
//
// A task may host several sides (e.g. the LVGL task consuming settings, meta
// and IQ), each with its own TaskRecipientT registered here.
//
// Binding: notifiers read the task handle when a side attaches, so bind()
// must be called before attachRecipient(). Construct inside the task and call
// bindToCurrentTask(), or construct anywhere and bind(handle) once the task
// exists.
//
// Recipients are added and removed only before the task starts dispatching,
// or from the task itself; this class does no locking.
//*****************************************************************************
class TaskDispatcher
{
public:
  static constexpr size_t MaxRecipients = MAKESDR_TASK_DISPATCHER_MAX_RECIPIENTS;

  TaskDispatcher() = default;
  explicit TaskDispatcher(TaskHandle_t task) : m_task(task) {}

  TaskDispatcher(const TaskDispatcher&) = delete;
  TaskDispatcher& operator=(const TaskDispatcher&) = delete;

  void bind(TaskHandle_t task) { m_task = task; }
  void bindToCurrentTask()     { m_task = xTaskGetCurrentTaskHandle(); }
  [[nodiscard]] TaskHandle_t taskHandle() const { return m_task; }

  // Returns false if MaxRecipients are already registered.
  bool add(IPayloadDispatch* recipient)
  {
    for (auto& slot : m_recipients) {
      if (slot == nullptr) {
        slot = recipient;
        return true;
      }
    }
    return false;
  }

  void remove(IPayloadDispatch* recipient)
  {
    for (auto& slot : m_recipients) {
      if (slot == recipient) {
        slot = nullptr;
      }
    }
  }

  //--- the bound task only ---------------------------------------------------

  // Blocks until woken or timeout, returning (and clearing) the bits set.
  [[nodiscard]] uint32_t wait(TickType_t timeout) { return TaskNotify::wait(timeout); }

  void dispatch(uint32_t bits)
  {
    PayloadTypeBits::forEach(bits, [this](PayloadType payloadType) {
      for (IPayloadDispatch* recipient : m_recipients) {
        if (recipient != nullptr) {
          recipient->dispatch(payloadType);
        }
      }
    });
  }

  // Returns true if anything woke the task before timeout.
  bool waitAndDispatch(TickType_t timeout)
  {
    const uint32_t bits = wait(timeout);
    dispatch(bits);
    return bits != 0;
  }

private:
  TaskHandle_t      m_task = nullptr;
  IPayloadDispatch* m_recipients[MaxRecipients] = {};
};
