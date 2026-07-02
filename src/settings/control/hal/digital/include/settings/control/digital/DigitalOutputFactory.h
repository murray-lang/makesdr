#pragma once

#include "DigitalOutputTypes.h"
#include "config/struct/DigitalOutputsConfig.h"

class DigitalOutputFactory
{
public:
  static ResultCode create(const Config::DigitalOutputs::DigitalOutputConfigVariant& config, DigitalOutputVariant& output);
};
