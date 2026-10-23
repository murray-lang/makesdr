#pragma once

#include "ConfigBase.h"

namespace Config::IqCapabilities
{
  struct Fields
  {
    bool dualPipeline{false};
    uint32_t sampleRate{0};
  };
};