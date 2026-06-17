#pragma once
#include "PttSink.h"
#include "settings/control/SettingsControlBase.h"
#include "settings/model/core/RadioSettingsSink.h"
#include "settings/model/core/SettingUpdateSink.h"

class SettingsControlSink :
  public SettingsControlBase,
  public RadioSettingsSink,
  public PttSink
{
};
