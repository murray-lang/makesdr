#pragma once
#include "settings/model/proto/RadioSettings.pb.h"

enum class SplitBandId
{
  None = makesdr_SplitBandId_SPLIT_BAND_NONE,
  One = makesdr_SplitBandId_SPLIT_BAND_ONE,
  Two = makesdr_SplitBandId_SPLIT_BAND_TWO
};
