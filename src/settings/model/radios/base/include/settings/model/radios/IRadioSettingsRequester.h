#pragma once
#include <ResultCode.h>
#include "SplitBandId.h"
#include "PipelineId.h"

class IRadioSettingsRequester
{
public:
  enum Requests
  {
    REQUEST_NONE,
    REQUEST_SETTINGS,
    REQUEST_MODES,
    REQUEST_BANDS
  };

  virtual ~IRadioSettingsRequester() = default;
  virtual ResultCode requestSettings() = 0;
  virtual ResultCode requestModes() = 0;
  virtual ResultCode requestBands() = 0;
};