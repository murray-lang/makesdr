#pragma once
#include <cstdint>
#include "IRadioSettings.h"

class IRadioSettingsEvent
{
public:
  virtual ~IRadioSettingsEvent() = default;
  virtual const IRadioSettings& getRadioSettings() const = 0;
  virtual int32_t getSequence() const = 0;
};
