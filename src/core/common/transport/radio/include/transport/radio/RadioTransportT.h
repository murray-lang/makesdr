#pragma once

#include <ResultCode.h>
#include <settings/model/message/PayloadType.h>
#include <settings/model/message/PayloadSource.h>
#include <settings/model/message/FieldUpdateMessage.h>
#include <settings/model/radios/iq/IqMessage.h>
#include <settings/model/data/mode/ModeList.h>
#include <settings/model/data/band/BandCategoryList.h>
#include <transport/MessageChannelSinkT.h>
#include <transport/MessageChannelSourceT.h>
#include "IqPublisher.h"

//*****************************************************************************
// The radio's end of a RadioMessageExchangeT, independent of platform.
//
// Outgoing messages are copied into the client-bound channels. Incoming
// messages are delivered to the connected sinks when the platform glue calls
// dispatch() on the radio's thread in response to a notification.
//
// The glue decides how and where the radio waits to be woken; it attaches its
// target with attach() and detaches with detach().
//*****************************************************************************
template<typename ExchangeT>
class RadioTransportT
{
public:
  using Settings = typename ExchangeT::Settings;


  explicit RadioTransportT(ExchangeT& exchange)
    : m_exchange(exchange)
    , m_settingsOut(exchange.settingsToClient())
    , m_iqOut(exchange.iqToClient())
    , m_modesOut(exchange.modesToClient())
    , m_bandsOut(exchange.bandsToClient())
    , m_settingsIn(exchange.settingsToRadio())
    , m_updateIn(exchange.updateToRadio())
  {
  }

  IqPublisher* getIqPublisher() { return &m_iqOut; }

  //--- outgoing (radio -> client) --------------------------------------------

  // Each message is stamped as coming from the back end before it is copied.
  ResultCode send(Settings* settings)       { return stampAndSend(m_settingsOut, settings); }
  ResultCode send(IqMessage* iq)            { return stampAndSend(m_iqOut, iq); }
  ResultCode send(ModeList* modes)          { return stampAndSend(m_modesOut, modes); }
  ResultCode send(BandCategoryList* bands)  { return stampAndSend(m_bandsOut, bands); }

  //--- incoming (client -> radio) --------------------------------------------

  void connectRadioSettingsSink(MessageSinkT<Settings>* sink)          { m_settingsIn.connectMessageSink(sink); }
  void connectFieldUpdateSink(MessageSinkT<FieldUpdateMessage>* sink)  { m_updateIn.connectMessageSink(sink); }

  // Radio thread only. Delivers whatever is pending on the channel the
  // payload type identifies; unknown types are ignored.
  void dispatch(PayloadType payloadType)
  {
    switch (static_cast<int>(payloadType)) {
      case Settings::payloadType:
        m_settingsIn.consumePending();
        break;
      case PAYLOAD_TYPE_FIELD_UPDATE:
        m_updateIn.consumePending();
        break;
      default:
        break;
    }
  }

  //--- for the platform glue -------------------------------------------------

  template<typename TargetT>
  void attach(TargetT target) { m_exchange.attachRadio(target); }
  void detach()               { m_exchange.detachRadio(); }

private:
  template<typename SinkT, typename MessageT>
  static ResultCode stampAndSend(SinkT& sink, MessageT* message)
  {
    message->setSource(PayloadSource::SOURCE_BACK_END);
    return sink.applyMessage(message);
  }

  ExchangeT& m_exchange;

  MessageChannelSinkT<typename ExchangeT::SettingsChannel> m_settingsOut;
  MessageChannelSinkT<typename ExchangeT::IqChannel>       m_iqOut;
  MessageChannelSinkT<typename ExchangeT::ModesChannel>    m_modesOut;
  MessageChannelSinkT<typename ExchangeT::BandsChannel>    m_bandsOut;

  MessageChannelSourceT<typename ExchangeT::SettingsChannel> m_settingsIn;
  MessageChannelSourceT<typename ExchangeT::UpdateChannel>   m_updateIn;
};
