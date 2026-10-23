#pragma once

#include <ResultCode.h>
#include <settings/model/radio/iq/IqMessage.h>
#include <settings/model/radio/iq/SpectrumMessage.h>
#include <transport/MessageRingSinkT.h>
#include "IqPublisher.h"

//*****************************************************************************
// The radio's end of an IqMessageRingsT, independent of platform.
//
// Adds the IQ rings to everything BasicRadioSideRingsT
// handles, and exposes the in-place publishers that pipeline stages write to.
// Nothing IQ-specific travels to the radio, so dispatch() is inherited.
//*****************************************************************************
template<typename RingsT>
class IqRadioSideRingsT
{
public:
  using Rings = RingsT;

  explicit IqRadioSideRingsT(RingsT& rings)
    : m_iqOut(rings.iqToClient())
  {
  }

  IqPublisher* getIqPublisher() { return &m_iqOut; }

  // Each message is stamped as coming from the back end before it is copied.
  ResultCode send(IqMessage* iq)            { return stampAndSend(m_iqOut, iq); }

protected:
  template<typename SinkT, typename MessageT>
  static ResultCode stampAndSend(SinkT& sink, MessageT* message)
  {
    message->setSource(PayloadSource::SOURCE_BACK_END);
    return sink.applyMessage(message);
  }

  MessageRingSinkT<typename RingsT::IqRing>       m_iqOut;
};
