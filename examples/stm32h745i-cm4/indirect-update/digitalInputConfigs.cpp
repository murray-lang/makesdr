#include "digitalInputConfigs.h"

Config::DigitalInput::Fields digitalInputConfig = {
  Config::Alternative {
    .type = Config::DigitalInput::type,
  },
  Config::GpioInputLines::Fields{
      Config::GpioLines::Fields{
        { 13 },
        "input",  // direction
        "none",   // bias
        "both",   // edge
      },
      false, // debounce
      true, //activeHigh
      false // rotaryencoder
    },
    "ptt" // settingPath
};

Config::DigitalInput::Fields rotaryEncoderConfig = {
  Config::Alternative {
    .type = Config::DigitalInput::type,
  },
  Config::GpioInputLines::Fields{
    Config::GpioLines::Fields{
          { 4, 8 },
          "input",  // direction
          "none",   // bias
          "both",   // edge
        },
        false, // debounce
        true, //activeHigh
        true // rotaryEncoder
      },
      "active_bands.focus_band.focus_pipeline.base.rf.centre_frequency" // settingPath
};

Config::DigitalInputs::Fields digitalInputsConfig = {
  Config::Alternative {
    .type = Config::DigitalInputs::type,
  },
  {digitalInputConfig, rotaryEncoderConfig}
};