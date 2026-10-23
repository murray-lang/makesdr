#pragma once

#include "RadioSettingsUpdater.h"
#include "IRadioSettingsView.h"

class IRadioSettingsMirror
{
public:
  virtual ~IRadioSettingsMirror() = default;
  virtual ResultCode start() = 0;
  virtual void stop() = 0;
  virtual RadioSettingsUpdater* updater() = 0;   // already type-neutral
  virtual ResultCode requestSettings() = 0;
  [[nodiscard]] virtual const IRadioSettingsView* view() const = 0;
};
