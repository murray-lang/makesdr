#pragma once
#include <ResultCode.h>
#include "Band.h"
#include <settings/model/meta/band/BandCategoryList.h>
#include <settings/model/meta/mode/ModeList.h>

class IApplyBandDefaults
{
public:
  virtual ResultCode applyBandDefaults(const Band& band, const BandCategoryList* bands, const ModeList* modes) = 0;
};
