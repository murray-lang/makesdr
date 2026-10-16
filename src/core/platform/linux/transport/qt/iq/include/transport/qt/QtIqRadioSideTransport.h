#pragma once

#include <QThread>
#include <ResultCode.h>
#include <settings/model/message/MessageSinkT.h>
#include <transport/IqPublisher.h>
#include <settings/model/radio/iq/FftMessage.h>
#include <transport/IqRadioSideRingsT.h>

#include "QtIqMessageRings.h"

class QtIqRadioSideTransport
{
public:
  using Rings = IqRadioSideRingsT<QtIqMessageRings>;

  QtIqRadioSideTransport(QtIqMessageRings& globalRings)
    : m_rings(globalRings)
  {
  }

  ~QtIqRadioSideTransport() = default;

  // Nothing to configure: the channels are fixed. Kept for existing callers.
  ResultCode configure() { return ResultCode::OK; }

  IqPublisher* getIqPublisher() { return m_rings.getIqPublisher(); }
  FftPublisher* getFftPublisher() { return m_rings.getFftPublisher(); }

  ResultCode send(IqMessage* iq)             { return m_rings.send(iq); }
  ResultCode send(FftMessage* fft)           { return m_rings.send(fft); }

private:

  Rings         m_rings;
};
