#pragma once

#include <transport/MessageRingT.h>
#include <settings/model/radio/RxMeteringMessage.h>

#define RX_METERING_RING_SIZE 2


//*****************************************************************************
// Ring between a radio and its client, owned by neither.
//
// Each channel carries messages one way only, since a ring has exactly one
// producer. Settings therefore have a channel in each direction.
//
// The radio and the client each attach to the channels addressed to them. A
// single target may attach to all of them at once: the payload type passed to
// the notifier identifies which channel to read. Never attach one target to
// both directions, as settings would be ambiguous.
//*****************************************************************************
template <typename NotifierT>
class RxMeteringMessageRingsT
{
public:
  using RxMeteringRing = MessageRingT<RxMeteringMessage, RX_METERING_RING_SIZE, NotifierT>;

  //--- client -> radio -------------------------------------------------------

  //--- radio -> client -------------------------------------------------------
  RxMeteringRing& rxMeteringToClient() { return m_rxMeteringToClient; }

protected:
  RxMeteringRing m_rxMeteringToClient;
};
