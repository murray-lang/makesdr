#pragma once
#include <transport/MessageRingT.h>
#include <settings/model/update/FieldUpdateMessage.h>

#define UPDATE_RING_SIZE 2


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
class UpdateMessageRingsT
{
public:
  using UpdateRing   = MessageRingT<FieldUpdateMessage, UPDATE_RING_SIZE, NotifierT>;

  //--- client -> radio -------------------------------------------------------
  UpdateRing&   updateToRadio()   { return m_updateToRadio; }

  //--- radio -> client -------------------------------------------------------

protected:
  UpdateRing   m_updateToRadio;
};
