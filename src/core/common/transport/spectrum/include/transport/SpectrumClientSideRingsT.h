#pragma once

#include <ResultCode.h>
#include <settings/model/message/PayloadType.h>
#include <settings/model/radio/iq/SpectrumMessage.h>
#include <transport/MessageRingSourceT.h>

#include <settings/model/message/MessageSinkT.h>

#include <transport/MetaClientSideRingsT.h>

//*****************************************************************************
// The client's end of an IqMessageRingsT, independent of platform: the mirror
// of IqRadioSideRingsT.
//
// Adds the IQ and FFT rings to everything BasicClientSideRingsT
// handles. Incoming messages are delivered to the connected sinks when the
// platform glue calls dispatch() on the client's thread in response to a
// notification.
//*****************************************************************************
template<typename RingsT>
class SpectrumClientSideRingsT
{
public:

  using Rings = RingsT;

  explicit SpectrumClientSideRingsT(RingsT& rings)
    : m_spectrumIn(rings.spectrumToClient())
  {
  }

  //--- incoming (radio -> client) --------------------------------------------

  void connectSpectrumSink(MessageSinkT<SpectrumMessage>* sink)             { m_spectrumIn.connectMessageSink(sink); }

  template<typename TargetT>
  void attachRecipient(TargetT target)
  {
    m_spectrumIn.attachRecipient(target);
  }

  void detachRecipient()
  {
    m_spectrumIn.detachRecipient();
  }

  // Client thread only. Delivers whatever is pending on the pipe the payload
  // type identifies, handing anything not IQ-specific to the base; unknown
  // types are ignored.
  void dispatch(PayloadType payloadType)
  {
    if (payloadType == PAYLOAD_TYPE_SPECTRUM ) {
      m_spectrumIn.consumePending();
    }
  }

private:
  MessageRingSourceT<typename RingsT::SpectrumRing> m_spectrumIn;
};
