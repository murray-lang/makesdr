#pragma once

#include "DigitalInput.h"
#include "config/struct/DigitalInputsConfig.h"

class DigitalInputFactory
{
public:
  static ResultCode create(const Config::DigitalInput::Fields& config, DigitalInput& input);
};
