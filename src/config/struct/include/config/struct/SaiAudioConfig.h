#pragma once
#include "ConfigBase.h"
#include <cstdint>

// STM32H745I-DISCO on-board audio: the WM8994 codec on SAI2, half duplex.
// The sample rate isn't configurable: it is fixed by the SAI class (48 kHz).
namespace Config::SaiAudio
{
  static constexpr auto type = "saiaudio";

  struct Fields : Alternative
  {
    uint32_t outputVolume = 70; // Headphone/line-out, 0..100
    uint32_t inputVolume = 75;  // Line-in (mic via external amp), 0..100. About 75-80 avoids clipping.
  };
}
