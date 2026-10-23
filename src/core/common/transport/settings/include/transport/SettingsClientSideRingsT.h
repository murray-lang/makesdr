#pragma once

#include <ResultCode.h>
#include <settings/model/message/PayloadType.h>
#include <settings/model/message/PayloadSource.h>
#include <transport/MessageRingSinkT.h>
#include <transport/MessageRingSourceT.h>
#include <settings/model/update/FieldUpdate.h>
#include <settings/model/update/FieldUpdateMessage.h>
#include <settings/model/update/FieldUpdateSink.h>

template<typename RingsT>
class SettingsClientSideRingsT
{
public:
  using Rings = RingsT;
  using Settings = typename RingsT::RadioSettings;

  explicit SettingsClientSideRingsT(RingsT& rings)
    : m_settingsOut(rings.settingsToRadio())
    , m_settingsIn(rings.settingsToClient())
  {
  }

  //--- outgoing (client -> radio) --------------------------------------------
  ResultCode send(Settings* settings)
  {
    settings->setSource(PayloadSource::SOURCE_FRONT_END);
    return m_settingsOut.applyMessage(settings);
  }

  //--- incoming (radio -> client) --------------------------------------------
  void connectRadioSettingsSink(MessageSinkT<Settings>* sink)     { m_settingsIn.connectMessageSink(sink); }

  // Client thread only. Delivers whatever is pending on the channel the
  // payload type identifies; unknown types are ignored.
  void dispatch(PayloadType payloadType)
  {
    if (payloadType == Settings::payloadType) {
      m_settingsIn.consumePending();
    }
  }

  //--- for the platform glue -------------------------------------------------

  template<typename TargetT>
  void attachRecipient(TargetT target) { m_settingsIn.attachRecipient(target); }
  void detachRecipient()               { m_settingsIn.detachRecipient(); }

private:
  MessageRingSinkT<typename RingsT::SettingsRing> m_settingsOut;
  MessageRingSourceT<typename RingsT::SettingsRing> m_settingsIn;
};
