#pragma once

#include "base/SettingsBase.h"
#include "band/ActiveBandSettings.h"
#include "ReceiverSettings.h"
#include "RadioSettings.h"
#include "TransmitterSettings.h"
#include "base/SettingsManipulator.h"
#include "util/DefaultRadioSettings.h"

class AutoCompleteRadioSettings : public RadioSettings
{
public:
  AutoCompleteRadioSettings()
    : m_manipulator(&m_rawSettings, &RadioSettings_RadioSettingsPb_msg)
  {
  }

  AutoCompleteRadioSettings(RadioSettings_RadioSettingsPb& raw)
  : RadioSettings(raw)
  , m_manipulator(&m_rawSettings, &RadioSettings_RadioSettingsPb_msg)
  {}

  ResultCode setMode(SplitBandId bandId, PipelineId pipelineId, const Mode::Type& mode);
  ResultCode setFocusPipelineMode(SplitBandId bandId, const Mode::Type& mode);
  ResultCode setFocusMode(const Mode::Type& mode);
  ResultCode setBand(SplitBandId bandId, const char* bandName);
  ResultCode splitBands(bool split, const NameString& newBandName);
  ResultCode setFocusBand(const char* bandName);

  ResultCode getFocusBandId(SplitBandId* pBandId);
  ResultCode getTxBandId(SplitBandId* pBandId);
  ResultCode getRxBandId(SplitBandId* pBandId);
  ResultCode getFocusPipelineId(SplitBandId bandId, PipelineId* pPipelineId);
  ResultCode isSplitBands(bool* pIsSplitBands);

protected:
  ResultCode getBandId(const SettingFieldPath& path, SplitBandId* pBandId);
  ResultCode getPipelineId(const SettingFieldPath& path, PipelineId* pPipelineId);
  ResultCode getBool(const SettingFieldPath& path, bool* pBool);

  SettingsManipulator m_manipulator;

  RadioSettings_BandCategoryListPb* m_pBandsPb;
  RadioSettings_ModeListPb* m_pModesPb;

};
