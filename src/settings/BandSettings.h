#pragma once
#include "base/SettingsBase.h"


class BandSettings : public SettingsBase
{
public:
  BandSettings(RadioSettings_BandSettingsPb& raw)
    : SettingsBase(&RadioSettings_BandSettingsPb_msg)
    , rawSettings(raw)
  {}
protected:
  RadioSettings_BandSettingsPb& rawSettings;
};


