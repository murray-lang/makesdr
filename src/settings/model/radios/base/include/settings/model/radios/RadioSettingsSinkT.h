#pragma once

#include <ResultCode.h>

template<typename RadioSettingsT>
class RadioSettingsSinkT
{
public:
  virtual ~RadioSettingsSinkT() = default;
  virtual ResultCode applySettings(RadioSettingsT& settings) = 0;
};
