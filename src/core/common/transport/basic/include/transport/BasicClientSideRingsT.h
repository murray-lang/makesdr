#pragma once

#include <ResultCode.h>
#include <settings/model/message/PayloadType.h>
#include <settings/model/message/PayloadSource.h>
#include <settings/model/update/FieldUpdate.h>
#include <settings/model/update/FieldUpdateMessage.h>
#include <settings/model/update/FieldUpdateSink.h>
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
class BasicClientSideRingsT : public FieldUpdateSink
{
public:
  explicit BasicClientSideRingsT(RingsT& rings)
    : m_pipes(rings)
    , m_updateOut(rings.updateToRadio())
    , m_capabilitiesIn(rings.capabilitiesToClient())
    , m_modesIn(rings.modesToClient())
    , m_bandsIn(rings.bandsToClient())
    , m_rxMeteringIn(rings.rxMeteringToClient())
  {
  }

  //--- outgoing (client -> radio) --------------------------------------------
  ResultCode send(FieldUpdateMessage* update)
  {
    update->setSource(PayloadSource::SOURCE_FRONT_END);
    return m_updateOut.applyMessage(update);
  }

  ResultCode applyFieldUpdate(const FieldUpdate& update) override
  {
    FieldUpdateMessage message(update, PayloadSource::SOURCE_FRONT_END);
    return m_updateOut.applyMessage(&message);
  }

  //--- incoming (radio -> client) --------------------------------------------

  void connectCapabilitiesSink(MessageSinkT<CapabilitiesMessage>* caps) { m_capabilitiesIn.connectMessageSink(caps); }
  void connectModesSink(MessageSinkT<ModeList>* sink)             { m_modesIn.connectMessageSink(sink); }
  void connectBandsSink(MessageSinkT<BandCategoryList>* sink)     { m_bandsIn.connectMessageSink(sink); }
  void connectRxMeteringSink(MessageSinkT<RxMeteringMessage>* sink)      { m_rxMeteringIn.connectMessageSink(sink); }

  // Client thread only. Delivers whatever is pending on the channel the
  // payload type identifies; unknown types are ignored.
  void dispatch(PayloadType payloadType)
  {
    switch (static_cast<int>(payloadType)) {
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
  void attach(TargetT target) { m_pipes.attachClient(target); }
  void detach()               { m_pipes.detachClient(); }

private:
  RingsT& m_pipes;

  MessageRingSinkT<typename RingsT::UpdateRing>   m_updateOut;

  MessageRingSourceT<typename RingsT::CapabilitiesRing>    m_capabilitiesIn;
  MessageRingSourceT<typename RingsT::ModesRing>    m_modesIn;
  MessageRingSourceT<typename RingsT::BandsRing>    m_bandsIn;
  MessageRingSourceT<typename RingsT::RxMeteringRing>    m_rxMeteringIn;
};
