#pragma once
#include <ResultCode.h>
#include "Band.h"
#include <settings/model/data/band/BandCategoryList.h>
#include <settings/model/data/mode/ModeList.h>

class IApplyBandDefaults
{
public:
  virtual ResultCode applyBandDefaults(const Band& band, const BandCategoryList* bands, const ModeList* modes) = 0;
};
