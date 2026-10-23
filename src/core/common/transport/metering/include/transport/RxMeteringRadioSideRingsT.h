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
// The radio's side of a MetaMessageRingsT, independent of platform.
//
// Outgoing messages are copied into the client-bound rings. Nothing meta
// travels to the radio yet, so this side only sends and needs no wake.
//*****************************************************************************
template<typename RingsT>
class RxMeteringRadioSideRingsT
{
public:
  using Rings = RingsT;

  explicit RxMeteringRadioSideRingsT(RingsT& rings)
    : m_rxMeteringOut(rings.rxMeteringToClient())
  {
  }

  RxMeteringPublisher* getRxMeteringPublisher()
  {
    return &this->m_rxMeteringOut;
  }

  //--- outgoing (radio -> client) --------------------------------------------

  // Each message is stamped as coming from the back end before it is copied.
  ResultCode send(RxMeteringMessage* metering) { return stampAndSend(m_rxMeteringOut, metering); }

protected:
  template<typename SinkT, typename MessageT>
  static ResultCode stampAndSend(SinkT& sink, MessageT* message)
  {
    message->setSource(PayloadSource::SOURCE_BACK_END);
    return sink.applyMessage(message);
  }

  MessageRingSinkT<typename RingsT::RxMeteringRing>   m_rxMeteringOut;
};
