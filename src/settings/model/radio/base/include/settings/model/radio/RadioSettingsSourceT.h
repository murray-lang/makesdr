#pragma once

#include "RadioSettingsSinkT.h"

template<typename RadioSettingsT>
class RadioSettingsSourceT
{
public:
  virtual ~RadioSettingsSourceT() = default;
  virtual void connectRadioSettingsSink(RadioSettingsSinkT<RadioSettingsT>* sink) = 0;

protected:
  virtual ResultCode notifySettings(RadioSettingsT& settings) = 0;
};

