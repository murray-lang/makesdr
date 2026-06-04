#pragma once

#include "ConfigBase.json.h"
#include "config/struct/BandSelectorBandsConfig.h"

namespace Config::BandSelector::Band
{
  extern ResultCode fromJson(const JsonVariantConst& json, Fields& fields);
}

// using BandSelectorBandsConfig = Config::BandSelector::Band::Fields;
