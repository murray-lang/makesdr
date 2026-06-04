#pragma once
#include "ConfigBase.h"

#include "AudioSignalIqSourceConfig.h"
#include <etl/variant.h>


namespace Config::IqIo
{
  static constexpr auto type = "iqio";

  using IqSourceConfigVariant = etl::variant<AudioSignalIqSource::Fields, AudioIqSource::Fields>;
  using AudioOutputConfigVariant = etl::variant<Audio::Fields>;

  struct Fields
  {
    IqSourceConfigVariant iqSource;
    AudioOutputConfigVariant audioOutput;
  };
}

// using IqIoConfig = Config::IqIo::Fields;
