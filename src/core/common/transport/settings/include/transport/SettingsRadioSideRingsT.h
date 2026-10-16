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


template<typename RingsT>
class SettingsRadioSideRingsT //: public IRadioPublishers
{
public:
  using Rings = RingsT;
  using RadioSettings = typename RingsT::RadioSettings;


  explicit SettingsRadioSideRingsT(RingsT& rings)
    : m_pipes(rings)
    , m_settingsOut(rings.settingsToClient())
    , m_settingsIn(rings.settingsToRadio())
  {
  }


  //--- outgoing (radio -> client) --------------------------------------------

  // Each message is stamped as coming from the back end before it is copied.
  ResultCode send(RadioSettings* settings)  { return stampAndSend(m_settingsOut, settings); }

  //--- incoming (client -> radio) --------------------------------------------
  void connectRadioSettingsSink(MessageSinkT<RadioSettings>* sink)  { m_settingsIn.connectMessageSink(sink); }

  // Radio thread only. Delivers whatever is pending on the channel the
  // payload type identifies; unknown types are ignored.
  void dispatch(PayloadType payloadType)
  {
    if (static_cast<int>(payloadType) == RadioSettings::payloadType) {
      m_settingsIn.consumePending();
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

  MessageRingSinkT<typename RingsT::SettingsRing> m_settingsOut;
  MessageRingSourceT<typename RingsT::SettingsRing> m_settingsIn;
};
