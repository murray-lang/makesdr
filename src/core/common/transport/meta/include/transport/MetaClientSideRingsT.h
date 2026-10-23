#pragma once

#include <ResultCode.h>
#include <settings/model/message/PayloadType.h>
#include <settings/model/message/PayloadSource.h>

#include <settings/model/radio/RxMeteringMessage.h>
#include <settings/model/radio/CapabilitiesMessage.h>
#include <settings/model/meta/mode/ModeList.h>
#include <settings/model/meta/band/BandCategoryList.h>
#include <transport/MessageRingSinkT.h>
#include <transport/MessageRingSourceT.h>

//*****************************************************************************
// The client's end of a BasicMessageRingsT, independent of platform: the
// mirror of BasicRadioSideRingsT.
//
// Outgoing settings and field updates are copied into the radio-bound
// rings, stamped as coming from the front end. Incoming messages are
// delivered to the connected sinks when the platform glue calls dispatch() on
// the client's thread in response to a notification.
//
// It is also a FieldUpdateSink, so an updater or requester can send field
// updates straight to the radio through it.
//*****************************************************************************
template<typename RingsT>
class MetaClientSideRingsT
{
public:
  using Rings = RingsT;

  explicit MetaClientSideRingsT(RingsT& rings)
    : m_capabilitiesIn(rings.capabilitiesToClient())
    , m_modesIn(rings.modesToClient())
    , m_bandsIn(rings.bandsToClient())
    , m_rxMeteringIn(rings.rxMeteringToClient())
  {
  }

  //--- outgoing (client -> radio) --------------------------------------------


  //--- incoming (radio -> client) --------------------------------------------

  void connectCapabilitiesSink(MessageSinkT<CapabilitiesMessage>* caps) { m_capabilitiesIn.connectMessageSink(caps); }
  void connectModesSink(MessageSinkT<ModeList>* sink)             { m_modesIn.connectMessageSink(sink); }
  void connectBandsSink(MessageSinkT<BandCategoryList>* sink)     { m_bandsIn.connectMessageSink(sink); }
  void connectRxMeteringSink(MessageSinkT<RxMeteringMessage>* sink)      { m_rxMeteringIn.connectMessageSink(sink); }

  // Client thread only. Delivers whatever is pending on the channel the
  // payload type identifies; unknown types are ignored.
  void dispatch(PayloadType payloadType)
  {
    switch (payloadType) {
    case PAYLOAD_TYPE_CAPABILITIES:
      m_capabilitiesIn.consumePending();
      break;
    case PAYLOAD_TYPE_MODES:
      m_modesIn.consumePending();
      break;
    case PAYLOAD_TYPE_BANDS:
      m_bandsIn.consumePending();
      break;
    case PAYLOAD_TYPE_RX_METERING:
      m_rxMeteringIn.consumePending();
      break;
    default:
      break;
    }
  }

  //--- for the platform glue -------------------------------------------------

  template<typename TargetT>
  void attachRecipient(TargetT target)
  {
    m_capabilitiesIn.attachRecipient(target);
    m_modesIn.attachRecipient(target);
    m_bandsIn.attachRecipient(target);
    m_rxMeteringIn.attachRecipient(target);
  }

  void detachRecipient()
  {
    m_capabilitiesIn.detachRecipient();
    m_modesIn.detachRecipient();
    m_bandsIn.detachRecipient();
    m_rxMeteringIn.detachRecipient();
  }

private:
  MessageRingSourceT<typename RingsT::CapabilitiesRing>    m_capabilitiesIn;
  MessageRingSourceT<typename RingsT::ModesRing>    m_modesIn;
  MessageRingSourceT<typename RingsT::BandsRing>    m_bandsIn;
  MessageRingSourceT<typename RingsT::RxMeteringRing>    m_rxMeteringIn;
};
