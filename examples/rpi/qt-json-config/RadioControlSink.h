#pragma once
#include "settings/model/core/RadioSettingsSink.h"
#include "settings/model/core/SettingFieldUpdateSink.h"


class RadioControlSink : public RadioSettingsSink, public SettingFieldUpdateSink
{
public:

  ResultCode applySettings(const RadioSettings& settings) override;
  ResultCode applySettingFieldUpdate(const SettingFieldUpdate& settingDelta) override;
};
