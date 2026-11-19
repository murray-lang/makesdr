#pragma once
#include <CrossPlatformTypes.h>
#include "AudioSignalIqSourceConfig.h"
#include "AdcIqSourceConfig.h"
#include "SaiAudioConfig.h"

#include <etl/variant.h>

namespace Config::IqIo
{
  static constexpr auto type = "iqio";

  using IqSourceConfigVariant = variant<
    AudioSignalIqSource::Fields,
    AudioIqSource::Fields,
    AdcIqSource::Fields
  >;
  using AudioOutputConfigVariant = variant<Audio::Fields, SaiAudio::Fields>;

  struct Fields
  {
    IqSourceConfigVariant iqSource;
    AudioOutputConfigVariant audioOutput;
  };
}

// using IqIoConfig = Config::IqIo::Fields;
