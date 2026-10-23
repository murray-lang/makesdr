#pragma once

#include <ResultCode.h>
#include <settings/model/message/PayloadType.h>
#include <settings/model/radio/iq/IqMessage.h>
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
class IqClientSideRingsT
{
public:
  using Rings = RingsT;

  explicit IqClientSideRingsT(RingsT& rings)
    : m_iqIn(rings.iqToClient())
  {
  }

  //--- incoming (radio -> client) --------------------------------------------

  void connectIqSink(MessageSinkT<IqMessage>* sink)               { m_iqIn.connectMessageSink(sink); }

  template<typename TargetT>
  void attachRecipient(TargetT target)
  {
    m_iqIn.attachRecipient(target);
  }

  void detachRecipient()
  {
    m_iqIn.detachRecipient();
  }

  // Client thread only. Delivers whatever is pending on the pipe the payload
  // type identifies, handing anything not IQ-specific to the base; unknown
  // types are ignored.
  void dispatch(PayloadType payloadType)
  {
    if (payloadType == PAYLOAD_TYPE_IQ) {
      m_iqIn.consumePending();
    }
  }

private:
  MessageRingSourceT<typename RingsT::IqRing>  m_iqIn;
};
