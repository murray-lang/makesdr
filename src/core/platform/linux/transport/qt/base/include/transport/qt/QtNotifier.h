#pragma once
#include <mutex>

#include <QCoreApplication>
#include <QEvent>
#include "MessageEvent.h"

//*****************************************************************************
// Wakes the consumer of a channel by posting it a MessageEvent whose event
// type is the payload type, telling the receiver which channel to read.
//
// The consumer attaches when ready and detaches when going away, from its own
// thread. While nothing is attached, notifications are dropped and messages
// wait in the ring.
//
// The mutex makes detach() a hard barrier: once it returns, no further event
// can be posted to the old target. Any event already posted but undelivered
// is discarded by ~QObject, and delivery and destruction both happen on the
// consumer's own thread, so a consumer that detaches in its destructor is
// safe.
//*****************************************************************************
class QtNotifier
{
public:
  void attach(QObject* target)
  {
    std::lock_guard lock(m_mutex);
    m_target = target;
  }

  void detach()
  {
    std::lock_guard lock(m_mutex);
    m_target = nullptr;
  }

  void operator()(PayloadType payloadType)
  {
    std::lock_guard lock(m_mutex);
    if (m_target != nullptr) {
      QCoreApplication::postEvent(m_target, new MessageEvent(payloadType));
    }
  }

private:
  std::mutex m_mutex;
  QObject*   m_target = nullptr;
};
