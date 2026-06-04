#pragma once

#include "ConfigBase.h"

#include "DigitalInputConfig.h"
#include "RotaryEncoderConfig.h"
#include "CrossPlatformTypes.h"


namespace Config::DigitalInputs
{
  static constexpr auto type = "digitalinputs";

  using DigitalInputConfigVariant = variant<DigitalInput::Fields, RotaryEncoder::Fields>;

#ifdef USE_ETL
  using DigitalInputConfigVector  = etl::vector<DigitalInputConfigVariant, MAX_DIGITAL_INPUT_HANDLERS>;
#else
  using DigitalInputConfigVector  = std::vector<DigitalInputConfigVariant>;
#endif

  struct Fields : Alternative
  {
    DigitalInputConfigVector inputs;
  };

}

// using DigitalInputsConfig = Config::DigitalInputs::Fields;
