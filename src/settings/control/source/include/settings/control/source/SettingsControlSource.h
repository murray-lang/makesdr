#pragma once
#include "settings/control/SettingsControlBase.h"
#include "settings/model/core/RadioSettingsSource.h"
#include "settings/model/core/RadioSettingsSink.h"
#include "settings/model/core/SettingUpdateSource.h"
#include "settings/model/core/SettingUpdateSink.h"

class SettingsControlSource :
  public SettingsControlBase,
  public RadioSettingsSource,
  public SettingUpdateSource
{
public:
  SettingsControlSource()
    : SettingsControlBase()
  {
  }
  SettingsControlSource(SettingsControlSource&& rhs) noexcept = default;
  ~SettingsControlSource() override = default;
  SettingsControlSource& operator=(SettingsControlSource&& rhs) noexcept = default;

  void connectRadioSettingsSink(RadioSettingsSink& sink) override
  {
    m_pSettingsSink.emplace(sink);
  }

  void connectSettingUpdateSink(SettingUpdateSink& sink) override
  {
    m_pFieldUpdateSink.emplace(sink);
  }
protected:
  ResultCode notifySettings(const RadioSettings& radioSettings) override
  {
    if (m_pSettingsSink) {
      return m_pSettingsSink->get().applySettings(radioSettings);
    }
    return ResultCode::OK;
  }


  ResultCode notifySettingUpdate(const SettingUpdate& update) override
  {
    if (m_pFieldUpdateSink) {
      m_pFieldUpdateSink->get().applySettingUpdate(update);
    }
    return ResultCode::OK;
  }

protected:
  optional<reference_wrapper<RadioSettingsSink>> m_pSettingsSink;
  optional<reference_wrapper<SettingUpdateSink>> m_pFieldUpdateSink;
};
