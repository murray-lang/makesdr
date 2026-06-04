#pragma once
#include "AudioIqSourceConfig.h"

namespace Config::AudioSignalIqSource
{
  static constexpr auto type = "audiosignaliqsource";

  struct Fields : AudioIqSource::Fields
  {
    bool reverse = false;
  };
}

// using AudioSignalIqSourceConfig = Config::AudioSignalIqSource::Fields;
