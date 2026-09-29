#pragma once

#include <settings/model/update/FieldUpdateMessage.h>
#include <settings/model/radio/iq/IqMessage.h>
#include <settings/model/meta/mode/ModeList.h>
#include <settings/model/meta/band/BandCategoryList.h>
#include <transport/MessageChannelT.h>

#define SETTINGS_RING_SIZE 2
#define UPDATE_RING_SIZE 4
#define IQ_RING_SIZE 4
#define MODES_RING_SIZE 1
#define BANDS_RING_SIZE 1

//*****************************************************************************
// Every channel between a radio and its client, owned by neither.
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
class RadioMessageExchangeT
{
public:
  using Settings        = RadioSettingsT;
  using SettingsChannel = MessageChannelT<RadioSettingsT, SETTINGS_RING_SIZE, NotifierT>;
  using UpdateChannel   = MessageChannelT<FieldUpdateMessage, UPDATE_RING_SIZE, NotifierT>;
  using IqChannel       = MessageChannelT<IqMessage, IQ_RING_SIZE, NotifierT>;
  using ModesChannel    = MessageChannelT<ModeList, MODES_RING_SIZE, NotifierT>;
  using BandsChannel    = MessageChannelT<BandCategoryList, BANDS_RING_SIZE, NotifierT>;

  //--- client -> radio -------------------------------------------------------

  SettingsChannel& settingsToRadio() { return m_settingsToRadio; }
  UpdateChannel&   updateToRadio()   { return m_updateToRadio; }

  //--- radio -> client -------------------------------------------------------

  SettingsChannel& settingsToClient() { return m_settingsToClient; }
  IqChannel&       iqToClient()       { return m_iqToClient; }
  ModesChannel&    modesToClient()    { return m_modesToClient; }
  BandsChannel&    bandsToClient()    { return m_bandsToClient; }

  //--- attaching consumers (only where NotifierT supports it) ----------------

  template<typename TargetT>
  void attachRadio(TargetT target)
  {
    m_settingsToRadio.notifier().attach(target);
    m_updateToRadio.notifier().attach(target);

    m_settingsToRadio.wakeIfPending();
    m_updateToRadio.wakeIfPending();
  }

  void detachRadio()
  {
    m_settingsToRadio.notifier().detach();
    m_updateToRadio.notifier().detach();
  }

  template<typename TargetT>
  void attachClient(TargetT target)
  {
    m_settingsToClient.notifier().attach(target);
    m_iqToClient.notifier().attach(target);
    m_modesToClient.notifier().attach(target);
    m_bandsToClient.notifier().attach(target);

    m_settingsToClient.wakeIfPending();
    m_iqToClient.wakeIfPending();
    m_modesToClient.wakeIfPending();
    m_bandsToClient.wakeIfPending();
  }

  void detachClient()
  {
    m_settingsToClient.notifier().detach();
    m_iqToClient.notifier().detach();
    m_modesToClient.notifier().detach();
    m_bandsToClient.notifier().detach();
  }

protected:
  SettingsChannel m_settingsToRadio;
  UpdateChannel   m_updateToRadio;

  SettingsChannel m_settingsToClient;
  IqChannel       m_iqToClient;
  ModesChannel    m_modesToClient;
  BandsChannel    m_bandsToClient;
};
