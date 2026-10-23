#pragma once

#include "RadioSettingsUpdater.h"
#include <settings/model/message/MessageSinkT.h>
#include <settings/model/message/PayloadPurpose.h>
#include "RadioRequester.h"

template<
  typename RadioSettingsT,
  typename SettingsTransportT,
  typename UpdateTransportT,
  typename ListenerT
>
class RadioSettingsMirrorT : public MessageSinkT<RadioSettingsT>
{
public:
  using RadioSettings = RadioSettingsT;

  RadioSettingsMirrorT(
    typename SettingsTransportT::Rings& settingsRings,
    UpdateTransportT& updateTransport,
    ListenerT* listener
    )
    : m_settingsTransport(settingsRings)
    , m_updateTransport(updateTransport)
    , m_listener(listener)
  {
    m_settingsTransport.connectRadioSettingsSink(this);
    m_settings.updater()->connectFieldUpdateSink(&m_updateTransport);
  }

  ResultCode applyMessage(RadioSettings* delta) override
  {
    if (delta == nullptr) return ResultCode::OK;
    const bool isReplace = delta->purpose() == PayloadPurpose::PURPOSE_REPLACE;
    ResultCode rc = isReplace
      ? m_settings.replace(*delta, true)
      : m_settings.merge(*delta);
    if (rc == ResultCode::OK) {
      // The listener gets the accumulated state and, separately, what changed. The has_* presence
      // of 'changes' is what downstream change checking relies on. A replace changes everything,
      // so the fully-present merged settings stand in for the changes.
      const RadioSettings& changes = isReplace ? m_settings : *delta;
      m_listener->template applySettings<RadioSettings>(m_settings, changes);
    }
    return rc;
  }

  ResultCode start() { m_settingsTransport.attachRecipient(); return ResultCode::OK; }
  void stop() { m_settingsTransport.detachRecipient(); }

  ResultCode requestSettings()
  {
    FieldDescriptor descriptor({ 0, RadioRequester::Requests::REQUEST_SETTINGS});
    return m_updateTransport.applyFieldUpdate(FieldUpdate(descriptor, true, FieldUpdateMeaning::VALUE));
  }


  RadioSettingsUpdater* updater() { return m_settings.updater(); }
  [[nodiscard]] const RadioSettings& settings() const { return m_settings; }

private:
  RadioSettings m_settings;
  SettingsTransportT m_settingsTransport;
  UpdateTransportT& m_updateTransport;
  ListenerT* m_listener;
};
