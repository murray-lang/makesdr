#pragma once
#include "AudioConfig.h"

namespace Config::AudioIqSource
{
  static constexpr auto type = "audioiqsource";

  struct Fields : Alternative
  {
    Audio::Fields audioInput;
  };
}

// using AudioIqSourceConfig = Config::AudioIqSource::Fields;
