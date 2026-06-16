#pragma once
#include "settings/model/core/RadioSettingsSink.h"
#include "settings/model/core/SettingUpdateSink.h"
#include <settings/model/core/RadioSettings.h>
#include "settings/model/core/RadioSettingsSource.h"

class RadioControlClient :
  public RadioSettingsSink,
  public SettingUpdateSink,
  public RadioSettingsSource
{
public:
  RadioControlClient();
  ResultCode applySettings(const RadioSettings& settings) override;
  ResultCode applySettingUpdate(const SettingUpdate& settingDelta) override;

  void connectRadioSettingsSink(RadioSettingsSink& sink) override
  {
    m_externalSettingsSink.emplace(sink);
  }

protected:
  ResultCode notifySettings(const RadioSettings& settings) override
  {
    if (m_externalSettingsSink) {
      return m_externalSettingsSink->get().applySettings(settings);
    }
    return ResultCode::OK;
  }


  RadioSettings m_radioSettings;
  optional<reference_wrapper<RadioSettingsSink>> m_externalSettingsSink;
};
