#pragma once

#include "ConfigBase.h"

#include "DigitalInputConfig.h"
#include "CrossPlatformTypes.h"


namespace Config::DigitalInputs
{
  static constexpr auto type = "digitalinputs";

#ifdef USE_ETL
  using DigitalInputConfigVector  = etl::vector<DigitalInput::Fields, MAX_DIGITAL_INPUT_HANDLERS>;
#else
  using DigitalInputConfigVector  = std::vector<DigitalInput::Fields>;
#endif

  struct Fields : Alternative
  {
    DigitalInputConfigVector inputs;
  };

}

// using DigitalInputsConfig = Config::DigitalInputs::Fields;
