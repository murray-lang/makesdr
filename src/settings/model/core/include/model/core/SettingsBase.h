#pragma once

class SettingsBase
{
public:
  SettingsBase() =default;
  virtual ~SettingsBase() = default;

  SettingsBase(const SettingsBase &rhs) = default;

  SettingsBase &operator=(const SettingsBase &rhs) = default;
};
