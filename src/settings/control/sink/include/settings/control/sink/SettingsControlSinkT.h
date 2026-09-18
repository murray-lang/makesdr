#pragma once
#include "PttSink.h"
#include "settings/control/SettingsControlBase.h"
#include <settings/model/radios/RadioSettingsSinkT.h>
#include "settings/model/message/FieldUpdateSink.h"

template <typename RadioSettingsT>
class SettingsControlSinkT :
  public SettingsControlBase,
  public RadioSettingsSinkT<RadioSettingsT>,
  public PttSink
{
};
