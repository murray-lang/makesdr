#pragma once

#include "TxCapabilitiesConfig.h"
#include "IqCapabilitiesConfig.h"
#include "FrequencyRangeConfig.h"

#ifdef USE_ETL
  using FrequencyRangeConfigVector  = etl::vector<Config::FrequencyRange::Fields, MAX_FREQUENCY_RANGES>;
#else
using FrequencyRangeConfigVector  = std::vector<Config::FrequencyRange::Fields>;
#endif

namespace Config::Capabilities
{
  static constexpr auto type = "capabilities";
  struct Fields
  {
    bool rx{true};
    optional<TxCapabilities::Fields> tx;
    optional<IqCapabilities::Fields> iq;
    FrequencyRangeConfigVector frequencyRanges;
  };
};