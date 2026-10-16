#pragma once

#include <settings/model/update/FieldUpdateMessage.h>
#include <settings/model/meta/mode/ModeList.h>
#include <settings/model/meta/band/BandCategoryList.h>
#include <transport/MessageRingT.h>
#include <settings/model/radio/RxMeteringMessage.h>
#include <settings/model/radio/CapabilitiesMessage.h>

#define CAPABILITIES_RING_SIZE 1
#define UPDATE_RING_SIZE 2
#define MODES_RING_SIZE 1
#define BANDS_RING_SIZE 1
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
class BasicMessageRingsT
{
public:
  using CapabilitiesRing = MessageRingT<CapabilitiesMessage, CAPABILITIES_RING_SIZE, NotifierT>;
  using UpdateRing   = MessageRingT<FieldUpdateMessage, UPDATE_RING_SIZE, NotifierT>;
  using ModesRing    = MessageRingT<ModeList, MODES_RING_SIZE, NotifierT>;
  using BandsRing    = MessageRingT<BandCategoryList, BANDS_RING_SIZE, NotifierT>;
  using RxMeteringRing = MessageRingT<RxMeteringMessage, RX_METERING_RING_SIZE, NotifierT>;

  //--- client -> radio -------------------------------------------------------
  UpdateRing&   updateToRadio()   { return m_updateToRadio; }

  //--- radio -> client -------------------------------------------------------
  CapabilitiesRing& capabilitiesToClient() { return m_capabilitiesToClient; }
  ModesRing&    modesToClient()    { return m_modesToClient; }
  BandsRing&    bandsToClient()    { return m_bandsToClient; }
  RxMeteringRing& rxMeteringToClient() { return m_rxMeteringToClient; }

  //--- attaching consumers (only where NotifierT supports it) ----------------

  template<typename TargetT>
  void attachRadio(TargetT target)
  {
    m_updateToRadio.notifier().attach(target);
    m_updateToRadio.wakeIfPending();
  }

  void detachRadio()
  {
    m_updateToRadio.notifier().detach();
  }

  template<typename TargetT>
  void attachClient(TargetT target)
  {
    m_capabilitiesToClient.notifier().attach(target);
    m_modesToClient.notifier().attach(target);
    m_bandsToClient.notifier().attach(target);
    m_rxMeteringToClient.notifier().attach(target);

    m_capabilitiesToClient.wakeIfPending();
    m_modesToClient.wakeIfPending();
    m_bandsToClient.wakeIfPending();
    m_rxMeteringToClient.wakeIfPending();
  }

  void detachClient()
  {
    m_capabilitiesToClient.notifier().detach();
    m_modesToClient.notifier().detach();
    m_bandsToClient.notifier().detach();
    m_rxMeteringToClient.notifier().detach();
  }

protected:
  UpdateRing   m_updateToRadio;

  CapabilitiesRing m_capabilitiesToClient;
  ModesRing    m_modesToClient;
  BandsRing    m_bandsToClient;
  RxMeteringRing m_rxMeteringToClient;
};
