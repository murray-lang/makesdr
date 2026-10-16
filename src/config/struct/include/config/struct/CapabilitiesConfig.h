#pragma once

#include "FrequencyRangesConfig.h"
#include "TxCapabilitiesConfig.h"
#include "IqCapabilitiesConfig.h"

namespace Config::Capabilities
{
  static constexpr auto type = "capabilities";
  struct Fields
  {
    bool rx{true};
    optional<TxCapabilities::Fields> tx;
    optional<IqCapabilities::Fields> iq;
    FrequencyRanges::Fields frequencyRanges;
  };
};