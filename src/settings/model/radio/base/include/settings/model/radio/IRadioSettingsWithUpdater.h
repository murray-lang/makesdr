#pragma once
#include "IRadioSettings.h"
#include "RadioSettingsUpdater.h"

class IRadioSettingsWithUpdater : public IRadioSettings //, public RadioSettingsShortcuts
{
public:
  virtual RadioSettingsUpdater* updater() { return nullptr; }
};
