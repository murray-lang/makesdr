#pragma once

#include "base/SettingsBase.h"
#include "band/ActiveBandSettings.h"
#include "ReceiverSettings.h"
#include "SimpleRadioSettings.h"
#include "TransmitterSettings.h"
#include "util/DefaultRadioSettings.h"

class SmartRadioSettings : public SimpleRadioSettings
{
public:
  SmartRadioSettings() = default;
  SmartRadioSettings(const RadioSettings_RadioSettingsPb& raw) : SimpleRadioSettings(raw) {}

  ResultCode setMode(SplitBandId bandId, PipelineId pipelineId, const Mode::Type& mode);
  ResultCode setFocusPipelineMode(SplitBandId bandId, const Mode::Type& mode);
  ResultCode setFocusMode(const Mode::Type& mode);
  ResultCode setBand(SplitBandId bandId, const char* bandName);
  ResultCode splitBands(bool split, const NameString& newBandName);
  ResultCode setFocusBand(const char* bandName);


protected:


};
