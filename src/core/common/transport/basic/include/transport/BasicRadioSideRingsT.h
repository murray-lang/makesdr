#pragma once

#include <ResultCode.h>
#include <settings/model/message/PayloadType.h>
#include <settings/model/message/PayloadSource.h>
#include <settings/model/update/FieldUpdateMessage.h>
#include <settings/model/meta/mode/ModeList.h>
#include <settings/model/meta/band/BandCategoryList.h>
#include <transport/MessageRingSinkT.h>
#include <transport/MessageRingSourceT.h>
#include <settings/model/radio/RxMeteringMessage.h>
#include <settings/model/radio/CapabilitiesMessage.h>


//*****************************************************************************
// The radio's end of a BasicMessageRingsT, independent of platform.
//
// Outgoing messages are copied into the client-bound rings. Incoming
// messages are delivered to the connected sinks when the platform glue calls
// dispatch() on the radio's thread in response to a notification.
//
// The glue decides how and where the radio waits to be woken; it attaches its
// target with attach() and detaches with detach().
//*****************************************************************************
template<typename RingsT>
class BasicRadioSideRingsT //: public IRadioPublishers
{
public:
  using Rings = RingsT;

  explicit BasicRadioSideRingsT(RingsT& rings)
    : m_pipes(rings)
    , m_capabilitiesOut(rings.capabilitiesToClient())
    , m_modesOut(rings.modesToClient())
    , m_bandsOut(rings.bandsToClient())
    , m_rxMeteringOut(rings.rxMeteringToClient())
    , m_updateIn(rings.updateToRadio())
  {
  }

  RxMeteringPublisher* getRxMeteringPublisher()
  {
    return &this->m_rxMeteringOut;
  }

  //--- outgoing (radio -> client) --------------------------------------------

  // Each message is stamped as coming from the back end before it is copied.
  ResultCode send(CapabilitiesMessage* caps)   { return stampAndSend(m_capabilitiesOut, caps); }
  ResultCode send(ModeList* modes)             { return stampAndSend(m_modesOut, modes); }
  ResultCode send(BandCategoryList* bands)     { return stampAndSend(m_bandsOut, bands); }
  ResultCode send(RxMeteringMessage* metering) { return stampAndSend(m_rxMeteringOut, metering); }

  //--- incoming (client -> radio) --------------------------------------------
  void connectFieldUpdateSink(MessageSinkT<FieldUpdateMessage>* sink)  { m_updateIn.connectMessageSink(sink); }

  // Radio thread only. Delivers whatever is pending on the channel the
  // payload type identifies; unknown types are ignored.
  void dispatch(PayloadType payloadType)
  {
    switch (static_cast<int>(payloadType)) {
    case PAYLOAD_TYPE_FIELD_UPDATE:
      m_updateIn.consumePending();
      break;
    default:
      break;
    }
  }

  template<typename TargetT>
  void attach(TargetT target) { m_pipes.attachRadio(target); }
  void detach()               { m_pipes.detachRadio(); }

protected:
  template<typename SinkT, typename MessageT>
  static ResultCode stampAndSend(SinkT& sink, MessageT* message)
  {
    message->setSource(PayloadSource::SOURCE_BACK_END);
    return sink.applyMessage(message);
  }

  RingsT& m_pipes;

  MessageRingSinkT<typename RingsT::CapabilitiesRing> m_capabilitiesOut;
  MessageRingSinkT<typename RingsT::ModesRing>        m_modesOut;
  MessageRingSinkT<typename RingsT::BandsRing>        m_bandsOut;
  MessageRingSinkT<typename RingsT::RxMeteringRing>   m_rxMeteringOut;

  MessageRingSourceT<typename RingsT::UpdateRing>   m_updateIn;
};
