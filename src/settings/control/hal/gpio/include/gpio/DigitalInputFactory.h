#pragma once

#include "DigitalInputTypes.h"
#include "config/DigitalInputsConfig.h"

class DigitalInputFactory
{
public:
  static ResultCode create(const Config::DigitalInputs::DigitalInputConfigVariant& config, DigitalInputVariant& input);
};
