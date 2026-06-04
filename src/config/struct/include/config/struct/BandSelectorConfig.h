#pragma once

#include "DigitalOutputConfig.h"
#include "BandSelectorBandsConfig.h"

#ifdef USE_ETL
#include <etl/vector.h>
#include <etl/optional.h>
#else
#include <vector>
#include <optional>
#endif


#define MAX_BAND_SELECTOR_BANDS 13

namespace Config::BandSelector
{
  static constexpr auto type = "bandselector";

#ifdef USE_ETL
  using OptionalBandConfig = etl::optional<Band::Fields>;
  using BandsVector = etl::vector<OptionalBandConfig, MAX_BAND_SELECTOR_BANDS>;
#else
  using OptionalBandConfig = std::optional<Band::Fields>;
  using BandsVector = std::vector<OptionalBandConfig>;
#endif

  struct Fields : DigitalOutput::Fields
  {
    uint32_t defaultOut{};
    BandsVector bands;
  };
}

// using BandSelectorConfig = Config::BandSelector::Fields;
