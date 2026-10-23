#pragma once

#include <settings/model/radio/iq/IqMessage.h>

#include <transport/MetaMessageRingsT.h>


#define IQ_RING_SIZE 1


//*****************************************************************************
// BasicMessageRingsT plus the IQ and FFT rings, both radio -> client.
//
// Each pipe carries messages one way only, since a ring has exactly one
// producer. The client attaches to all of its rings at once, and the payload
// type passed to the notifier identifies which pipe to read.
//*****************************************************************************
template <typename NotifierT>
class IqMessageRingsT //: public BasicMessageRingsT<RadioSettingsT, NotifierT>
{
  // using Base = BasicMessageRingsT<RadioSettingsT, NotifierT>;

public:
  using IqRing       = MessageRingT<IqMessage, IQ_RING_SIZE, NotifierT>;

  //--- radio -> client -------------------------------------------------------

  IqRing&       iqToClient()       { return m_iqToClient; }

protected:
  IqRing       m_iqToClient;
};
