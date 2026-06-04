#pragma once

#include "DigitalInputConfig.h"

namespace Config::RotaryEncoder
{
  static constexpr auto type = "rotaryencoder";

  struct Fields : DigitalInput::Fields {};
}

// using RotaryEncoderConfig = Config::RotaryEncoder::Fields;

