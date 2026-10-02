#pragma once
#include "PttSink.h"
#include "settings/control/SettingsControlBase.h"
#include <settings/model/radio/RadioSettingsSinkT.h>
#include "settings/model/update/FieldUpdateSink.h"

template <typename RadioSettingsT>
class SettingsControlSinkT :
  public SettingsControlBase,
  public RadioSettingsSinkT<RadioSettingsT>,
  public PttSink
{
};
