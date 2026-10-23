#pragma once

#include <settings/model/radio/iq/IqMessage.h>
#include <settings/model/radio/iq/SpectrumMessage.h>

#include <transport/MessageRingT.h>

#define SPECTRUM_RING_SIZE 1


//*****************************************************************************
// BasicMessageRingsT plus the IQ and FFT rings, both radio -> client.
//
// Each pipe carries messages one way only, since a ring has exactly one
// producer. The client attaches to all of its rings at once, and the payload
// type passed to the notifier identifies which pipe to read.
//*****************************************************************************
template <typename NotifierT>
class SpectrumMessageRingsT
{
public:
  using SpectrumRing = MessageRingT<SpectrumMessage, SPECTRUM_RING_SIZE, NotifierT>;

  //--- radio -> client -------------------------------------------------------

  SpectrumRing& spectrumToClient()      { return m_spectrumToClient; }

protected:
  SpectrumRing m_spectrumToClient;
};
