#pragma once

#include <ResultCode.h>
#include <settings/model/radio/iq/SpectrumMessage.h>
#include <transport/MessageRingSinkT.h>
#include "SpectrumPublisher.h"


//*****************************************************************************
// The radio's end of an IqMessageRingsT, independent of platform.
//
// Adds the IQ and FFT rings to everything BasicRadioSideRingsT
// handles, and exposes the in-place publishers that pipeline stages write to.
// Nothing IQ-specific travels to the radio, so dispatch() is inherited.
//*****************************************************************************
template<typename RingsT>
class SpectrumRadioSideRingsT
{
public:
  using Rings = RingsT;

  explicit SpectrumRadioSideRingsT(RingsT& rings)
    : m_spectrumOut(rings.spectrumToClient())
  {
  }

  SpectrumPublisher* getSpectrumPublisher() { return &m_spectrumOut; }

  // Each message is stamped as coming from the back end before it is copied.
  ResultCode send(SpectrumMessage* fft)          { return stampAndSend(m_spectrumOut, fft); }

protected:
  template<typename SinkT, typename MessageT>
  static ResultCode stampAndSend(SinkT& sink, MessageT* message)
  {
    message->setSource(PayloadSource::SOURCE_BACK_END);
    return sink.applyMessage(message);
  }

  MessageRingSinkT<typename RingsT::SpectrumRing> m_spectrumOut;
};
