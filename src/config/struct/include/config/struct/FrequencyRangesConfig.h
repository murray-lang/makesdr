#pragma once
#include "FrequencyRangeConfig.h"

namespace Config::FrequencyRanges
{
#ifdef USE_ETL
  using FrequencyRangeConfigVector  = etl::vector<FrequencyRange::Fields, MAX_FREQUENCY_RANGES>;
#else
  using FrequencyRangeConfigVector  = std::vector<FrequencyRange::Fields>;
#endif

  struct Fields
  {
    FrequencyRangeConfigVector frequencyRanges{};
  };
};