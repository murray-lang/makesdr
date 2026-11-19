#include "digitalInputConfigs.h"

// Base order matters: DigitalInput::Fields : GpioInputLines::Fields, Alternative
Config::DigitalInput::Fields digitalInputConfig = {
  Config::GpioInputLines::Fields{
    Config::GpioLines::Fields{
      { 13 },   // lines
      "input",  // direction
      "none",   // bias
      "both",   // edge
    },
    false, // debounce
    true,  // activeHigh
    false  // rotaryEncoder
  },
  Config::Alternative{ Config::DigitalInput::type },
  "ptt" // settingPath
};

Config::DigitalInput::Fields rotaryEncoderConfig = {
  Config::GpioInputLines::Fields{
    Config::GpioLines::Fields{
      { 4, 8 }, // lines
      "input",  // direction
      "none",   // bias
      "both",   // edge
    },
    false, // debounce
    true,  // activeHigh
    true   // rotaryEncoder
  },
  Config::Alternative{ Config::DigitalInput::type },
  "active_bands.focus_band.focus_pipeline.base.rf.frequency" // settingPath
};

// Aggregates with base classes can't use designated initialisers, so it's all positional.
Config::DigitalInputs::Fields digitalInputsConfig = {
  Config::Alternative{ Config::DigitalInputs::type },
  { digitalInputConfig, rotaryEncoderConfig } // inputs
};
