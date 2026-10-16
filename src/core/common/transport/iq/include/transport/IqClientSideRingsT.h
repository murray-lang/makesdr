#pragma once

#include <ResultCode.h>
#include <settings/model/message/PayloadType.h>
#include <settings/model/radio/iq/IqMessage.h>
#include <settings/model/radio/iq/FftMessage.h>
#include <transport/MessageRingSourceT.h>

#include <settings/model/message/MessageSinkT.h>

#include <transport/BasicClientSideRingsT.h>

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

  explicit IqClientSideRingsT(RingsT& rings)
    : m_pipes(rings)
    , m_iqIn(rings.iqToClient())
    , m_fftIn(rings.fftToClient())
  {
  }

  //--- incoming (radio -> client) --------------------------------------------

  void connectIqSink(MessageSinkT<IqMessage>* sink)               { m_iqIn.connectMessageSink(sink); }
  void connectFftSink(MessageSinkT<FftMessage>* sink)             { m_fftIn.connectMessageSink(sink); }

  template<typename TargetT>
  void attach(TargetT target) { m_pipes.attachClient(target); }
  void detach()               { m_pipes.detachClient(); }

  // Client thread only. Delivers whatever is pending on the pipe the payload
  // type identifies, handing anything not IQ-specific to the base; unknown
  // types are ignored.
  void dispatch(PayloadType payloadType)
  {
    switch (static_cast<int>(payloadType)) {
    case PAYLOAD_TYPE_IQ:
      m_iqIn.consumePending();
      break;
    case PAYLOAD_TYPE_FFT:
      m_fftIn.consumePending();
      break;
    default:
      break;
    }
  }

private:

  RingsT& m_pipes;

  MessageRingSourceT<typename RingsT::IqRing>  m_iqIn;
  MessageRingSourceT<typename RingsT::FftRing> m_fftIn;
};
