#pragma once
#include "settings/control/SettingsControlBase.h"
#include "settings/model/core/RadioSettingsSource.h"
#include "settings/model/core/RadioSettingsSink.h"
#include "settings/model/core/SettingFieldUpdateSource.h"
#include "settings/model/core/SettingFieldUpdateSink.h"

class SettingsControlSource :
  public SettingsControlBase,
  public RadioSettingsSource,
  public SettingFieldUpdateSource
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

  void connectSettingFieldUpdateSink(SettingFieldUpdateSink& sink) override
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


  ResultCode notifySettingFieldUpdate(const SettingFieldUpdate& update) override
  {
    if (m_pFieldUpdateSink) {
      m_pFieldUpdateSink->get().applySettingFieldUpdate(update);
    }
    return ResultCode::OK;
  }

protected:
  optional<reference_wrapper<RadioSettingsSink>> m_pSettingsSink;
  optional<reference_wrapper<SettingFieldUpdateSink>> m_pFieldUpdateSink;
};
