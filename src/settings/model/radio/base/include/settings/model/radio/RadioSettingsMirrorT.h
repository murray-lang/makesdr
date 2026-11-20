#pragma once

#include "RadioSettingsUpdater.h"
#include <settings/model/message/MessageSinkT.h>
#include <settings/model/message/PayloadPurpose.h>
#include "RadioRequester.h"

//*****************************************************************************
// The client's copy of the radio's settings, kept current from the settings
// the radio sends, and the route by which local changes go back to it.
//
// Uses the client transport rather than owning a settings side of its own, so
// there is only ever one receiver on the settings rings. The transport must
// outlive the mirror, and it decides when settings are received (e.g. by
// attaching its recipients when its task starts), so the mirror has no
// start()/stop().
//
// TransportT provides:
//   RadioSettings, UpdateTransport
//   connectRadioSettingsSink(MessageSinkT<RadioSettings>*)
//   UpdateTransport& getUpdateTransport()
//*****************************************************************************
template<typename TransportT>
class RadioSettingsMirrorT : public MessageSinkT<typename TransportT::RadioSettings>
{
public:
  using RadioSettings = typename TransportT::RadioSettings;
  using UpdateTransport = typename TransportT::UpdateTransport;

  class IListener
  {
  public:
    virtual void radioSettingsChanged(const RadioSettings& state, const RadioSettings& changes) = 0;
  protected:
    ~IListener() = default;
  };

  RadioSettingsMirrorT(TransportT& transport, IListener* listener)
    : m_updateTransport(transport.getUpdateTransport())
    , m_listener(listener)
  {
    transport.connectRadioSettingsSink(this);
    RadioSettingsUpdater* updater = m_settings.updater();
    if (updater != nullptr) {
      updater->connectFieldUpdateSink(&m_updateTransport);
    }
  }

  RadioSettingsMirrorT(const RadioSettingsMirrorT&) = delete;
  RadioSettingsMirrorT& operator=(const RadioSettingsMirrorT&) = delete;

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
      m_listener->radioSettingsChanged(m_settings, changes);
    }
    return rc;
  }

  ResultCode requestSettings()
  {
    FieldDescriptor descriptor({ 0, RadioRequester::Requests::REQUEST_SETTINGS});
    return m_updateTransport.applyFieldUpdate(FieldUpdate(descriptor, true, FieldUpdateMeaning::VALUE));
  }

  RadioSettingsUpdater* updater() { return m_settings.updater(); }
  [[nodiscard]] const RadioSettings& settings() const { return m_settings; }

private:
  RadioSettings m_settings;
  UpdateTransport& m_updateTransport;
  IListener* m_listener;
};
