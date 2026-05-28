#pragma once
#include "PttSink.h"
#include "control/SettingsControlBase.h"
#include "../../../../../model/basic/include/model/basic/BasicRadioSettingsSink.h"

class SettingsControlSink :
  public SettingsControlBase,
  public BasicRadioSettingsSink, public PttSink
{
};
