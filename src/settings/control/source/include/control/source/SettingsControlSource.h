#pragma once
#include "control/SettingsControlBase.h"
#include "../../../../../model/basic/include/model/basic/BasicRadioSettingsSource.h"
#include "model/SettingFieldUpdateSource.h"

class SettingsControlSource :
  public SettingsControlBase,
  public BasicRadioSettingsSource,
  public SettingFieldUpdateSource
{
public:
  ~SettingsControlSource() override = default;

};
