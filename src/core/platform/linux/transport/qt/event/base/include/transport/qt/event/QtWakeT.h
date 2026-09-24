#pragma once

#include <QEvent>
#include <QObject>

#include <settings/model/message/PayloadType.h>

//*****************************************************************************
// The Qt glue that wakes a transport: a QObject that QtNotifier posts
// MessageEvents to, and which passes each one's payload type to the
// transport's dispatch() on whatever thread this object lives in.
//
// It owns no thread. Move it to the thread that should do the consuming
// before attaching it (the radio's QThread, or leave it on the GUI thread
// for the client).
//*****************************************************************************
template<typename TransportT>
class QtWakeT : public QObject
{
public:
  explicit QtWakeT(TransportT& transport)
    : m_transport(transport)
  {
  }

  bool event(QEvent* e) override
  {
    if (e->type() < QEvent::User) {
      return QObject::event(e);
    }
    m_transport.dispatch(static_cast<PayloadType>(e->type() - QEvent::User));
    return true;
  }

private:
  TransportT& m_transport;
};
