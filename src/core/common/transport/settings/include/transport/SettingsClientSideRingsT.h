#pragma once

#include <ResultCode.h>
#include <settings/model/message/PayloadType.h>
#include <settings/model/message/PayloadSource.h>
#include <transport/MessageRingSinkT.h>
#include <transport/MessageRingSourceT.h>

template<typename RingsT>
class SettingsClientSideRingsT
{
public:
  using Settings = typename RingsT::RadioSettings;

  explicit SettingsClientSideRingsT(RingsT& rings)
    : m_pipes(rings)
    , m_settingsOut(rings.settingsToRadio())
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
    if (static_cast<int>(payloadType) == Settings::payloadType) {
      m_settingsIn.consumePending();
    }
  }

  //--- for the platform glue -------------------------------------------------

  template<typename TargetT>
  void attach(TargetT target) { m_pipes.attachClient(target); }
  void detach()               { m_pipes.detachClient(); }

private:
  RingsT& m_pipes;

  MessageRingSinkT<typename RingsT::SettingsRing> m_settingsOut;
  MessageRingSourceT<typename RingsT::SettingsRing> m_settingsIn;
};
