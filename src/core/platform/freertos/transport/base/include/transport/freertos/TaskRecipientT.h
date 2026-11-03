#pragma once

#include <settings/model/message/PayloadType.h>

#include "TaskDispatcher.h"

//*****************************************************************************
// The FreeRTOS glue that notifies a transport side: the counterpart of
// QtRecipientT. Registers with the TaskDispatcher of the task that consumes,
// and passes each payload type that task is woken for to the side's
// dispatch(), on that task.
//
// Notifiers (TaskNotifier, HsemNotifier) are attached to it, and take the
// task to wake from taskHandle().
//
// Normally owned by a RecipientSideT:
//     RecipientSideT<MySideT, TaskRecipientT> side(rings, dispatcher);
//*****************************************************************************
template<typename RingsInterfaceT>
class TaskRecipientT : public IPayloadDispatch
{
public:
  TaskRecipientT(RingsInterfaceT& side, TaskDispatcher& dispatcher)
    : m_side(side)
    , m_dispatcher(dispatcher)
  {
    const bool added = m_dispatcher.add(this);
    configASSERT(added);  // raise MAKESDR_TASK_DISPATCHER_MAX_RECIPIENTS
    (void)added;
  }

  ~TaskRecipientT() { m_dispatcher.remove(this); }

  TaskRecipientT(const TaskRecipientT&) = delete;
  TaskRecipientT& operator=(const TaskRecipientT&) = delete;

  [[nodiscard]] TaskHandle_t taskHandle() const { return m_dispatcher.taskHandle(); }

  void dispatch(PayloadType payloadType) override { m_side.dispatch(payloadType); }

private:
  RingsInterfaceT& m_side;
  TaskDispatcher&  m_dispatcher;
};
