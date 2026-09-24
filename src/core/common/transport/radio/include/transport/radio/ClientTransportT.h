#pragma once

#include <ResultCode.h>
#include <settings/model/message/PayloadType.h>
#include <settings/model/message/PayloadSource.h>
#include <settings/model/message/FieldUpdate.h>
#include <settings/model/message/FieldUpdateMessage.h>
#include <settings/model/message/FieldUpdateSink.h>
#include <settings/model/radios/iq/IqMessage.h>
#include <settings/model/data/mode/ModeList.h>
#include <settings/model/data/band/BandCategoryList.h>
#include <transport/MessageChannelSinkT.h>
#include <transport/MessageChannelSourceT.h>

//*****************************************************************************
// The client's end of a RadioMessageExchangeT, independent of platform: the
// mirror of RadioTransportT.
//
// Outgoing settings and field updates are copied into the radio-bound
// channels, stamped as coming from the front end. Incoming messages are
// delivered to the connected sinks when the platform glue calls dispatch() on
// the client's thread in response to a notification.
//
// It is also a FieldUpdateSink, so an updater or requester can send field
// updates straight to the radio through it.
//*****************************************************************************
template<typename ExchangeT>
class ClientTransportT : public FieldUpdateSink
{
public:
  using Settings = typename ExchangeT::Settings;

  explicit ClientTransportT(ExchangeT& exchange)
    : m_exchange(exchange)
    , m_settingsOut(exchange.settingsToRadio())
    , m_updateOut(exchange.updateToRadio())
    , m_settingsIn(exchange.settingsToClient())
    , m_iqIn(exchange.iqToClient())
    , m_modesIn(exchange.modesToClient())
    , m_bandsIn(exchange.bandsToClient())
  {
  }

  //--- outgoing (client -> radio) --------------------------------------------

  ResultCode send(Settings* settings)
  {
    settings->setSource(PayloadSource::SOURCE_FRONT_END);
    return m_settingsOut.applyMessage(settings);
  }

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

  void connectRadioSettingsSink(MessageSinkT<Settings>* sink)     { m_settingsIn.connectMessageSink(sink); }
  void connectIqSink(MessageSinkT<IqMessage>* sink)               { m_iqIn.connectMessageSink(sink); }
  void connectModesSink(MessageSinkT<ModeList>* sink)             { m_modesIn.connectMessageSink(sink); }
  void connectBandsSink(MessageSinkT<BandCategoryList>* sink)     { m_bandsIn.connectMessageSink(sink); }

  // Client thread only. Delivers whatever is pending on the channel the
  // payload type identifies; unknown types are ignored.
  void dispatch(PayloadType payloadType)
  {
    switch (static_cast<int>(payloadType)) {
      case Settings::payloadType:
        m_settingsIn.consumePending();
        break;
      case PAYLOAD_TYPE_IQ:
        m_iqIn.consumePending();
        break;
      case PAYLOAD_TYPE_MODES:
        m_modesIn.consumePending();
        break;
      case PAYLOAD_TYPE_BANDS:
        m_bandsIn.consumePending();
        break;
      default:
        break;
    }
  }

  //--- for the platform glue -------------------------------------------------

  template<typename TargetT>
  void attach(TargetT target) { m_exchange.attachClient(target); }
  void detach()               { m_exchange.detachClient(); }

private:
  ExchangeT& m_exchange;

  MessageChannelSinkT<typename ExchangeT::SettingsChannel> m_settingsOut;
  MessageChannelSinkT<typename ExchangeT::UpdateChannel>   m_updateOut;

  MessageChannelSourceT<typename ExchangeT::SettingsChannel> m_settingsIn;
  MessageChannelSourceT<typename ExchangeT::IqChannel>       m_iqIn;
  MessageChannelSourceT<typename ExchangeT::ModesChannel>    m_modesIn;
  MessageChannelSourceT<typename ExchangeT::BandsChannel>    m_bandsIn;
};
