#pragma once

#include <ResultCode.h>
#include <transport/qt/QtWakeT.h>
#include <transport/IqClientSideRingsT.h>
#include "QtIqMessageRings.h"


class QtIqClientSideTransport
{
public:
  using Rings = IqClientSideRingsT<QtIqMessageRings>;

  QtIqClientSideTransport(QtIqMessageRings& globalRings)
    : m_rings(globalRings)
    , m_wake(m_rings)
  {
  }

  ~QtIqClientSideTransport()
  {
    detach();
  }

  void moveToThread(QThread* thread) { m_wake.moveToThread(thread); }

  // Nothing to configure: the channels are fixed. Kept for existing callers.
  ResultCode configure() { return ResultCode::OK; }

  void connectIqSink(MessageSinkT<IqMessage>* sink)     { m_rings.connectIqSink(sink); }
  void connectFftSink(MessageSinkT<FftMessage>* sink)   { m_rings.connectFftSink(sink); }

  ResultCode attach()
  {
    m_rings.attach(&m_wake);
    return ResultCode::OK;
  }

  void detach() { m_rings.detach(); }

private:
  Rings         m_rings;
  QtWakeT<Rings> m_wake;
};
