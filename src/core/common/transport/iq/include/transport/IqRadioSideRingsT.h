#pragma once

#include <ResultCode.h>
#include <settings/model/radio/iq/IqMessage.h>
#include <settings/model/radio/iq/FftMessage.h>
#include <transport/MessageRingSinkT.h>
#include <transport/MessageRingSourceT.h>
#include <transport/RxMeteringPublisher.h>
#include "IqPublisher.h"
#include "FftPublisher.h"
#include "IRadioPublishers.h"
#include <transport/BasicRadioSideRingsT.h>


//*****************************************************************************
// The radio's end of an IqMessageRingsT, independent of platform.
//
// Adds the IQ and FFT rings to everything BasicRadioSideRingsT
// handles, and exposes the in-place publishers that pipeline stages write to.
// Nothing IQ-specific travels to the radio, so dispatch() is inherited.
//*****************************************************************************
template<typename RingsT>
class IqRadioSideRingsT //: public IRadioPublishers ,  public BasicRadioSideRingsT<RingsT>
{
public:
  using Rings = RingsT;

  // The IQ overloads below would otherwise hide the base's.

  explicit IqRadioSideRingsT(RingsT& rings)
    : m_pipes(rings)
    , m_iqOut(rings.iqToClient())
    , m_fftOut(rings.fftToClient())
  {
  }


  IqPublisher* getIqPublisher() { return &m_iqOut; }
  FftPublisher* getFftPublisher() { return &m_fftOut; }

  // template<typename TargetT>
  // void attach(TargetT target) { m_pipes.attachRadio(target); }
  // void detach()               { m_pipes.detachRadio(); }

  // Each message is stamped as coming from the back end before it is copied.
  ResultCode send(IqMessage* iq)            { return stampAndSend(m_iqOut, iq); }
  ResultCode send(FftMessage* fft)          { return stampAndSend(m_fftOut, fft); }

protected:
  template<typename SinkT, typename MessageT>
  static ResultCode stampAndSend(SinkT& sink, MessageT* message)
  {
    message->setSource(PayloadSource::SOURCE_BACK_END);
    return sink.applyMessage(message);
  }

  RingsT& m_pipes;

  MessageRingSinkT<typename RingsT::IqRing>     m_iqOut;
  MessageRingSinkT<typename RingsT::FftRing>    m_fftOut;
};
