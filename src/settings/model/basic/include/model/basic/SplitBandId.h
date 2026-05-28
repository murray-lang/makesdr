#pragma once
#include "model/proto/RadioSettings.pb.h"

enum class SplitBandId
{
  None = RadioSettings_SplitBandId_SPLIT_BAND_NONE,
  One = RadioSettings_SplitBandId_SPLIT_BAND_ONE,
  Two = RadioSettings_SplitBandId_SPLIT_BAND_TWO
};
