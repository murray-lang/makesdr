#pragma once
#include <transport/MessageRingT.h>
#include <settings/model/update/FieldUpdateMessage.h>

#define SETTINGS_RING_SIZE 1


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
template <typename RadioSettingsT, typename NotifierT>
class SettingsMessageRingsT
{
public:
  using RadioSettings     = RadioSettingsT;
  using SettingsRing = MessageRingT<RadioSettingsT, SETTINGS_RING_SIZE, NotifierT>;

  //--- client -> radio -------------------------------------------------------
  SettingsRing& settingsToRadio() { return m_settingsToRadio; }

  //--- radio -> client -------------------------------------------------------
  SettingsRing& settingsToClient() { return m_settingsToClient; }

protected:
  SettingsRing m_settingsToRadio;
  SettingsRing m_settingsToClient;
};
