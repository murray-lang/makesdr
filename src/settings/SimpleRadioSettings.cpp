#include "SimpleRadioSettings.h"
#include "util/ResolvedFieldPaths.h"
#include "mode/ModeList.h"
#include "band/AvailableBands.h"
#include "band/BandCategoryList.h"

ResultCode
SimpleRadioSettings::getFocusBandId(SplitBandId* pBandId)
{
  return getBandId(active_bands_focus_band_id, pBandId);
}

ResultCode
SimpleRadioSettings::getTxBandId(SplitBandId* pBandId)
{
  return getBandId(active_bands_tx_band_id, pBandId);
}
ResultCode
SimpleRadioSettings::getRxBandId(SplitBandId* pBandId)
{
  return getBandId(active_bands_rx_band_id, pBandId);
}

ResultCode
SimpleRadioSettings::getFocusPipelineId(SplitBandId bandId, PipelineId* pPipelineId)
{
  const SettingFieldPath* focusPipelineIdPath = nullptr;

  if (bandId == SplitBandId::One) {
    focusPipelineIdPath = &active_bands_band_1_focus_pipeline_id;
  } else if (bandId == SplitBandId::Two) {
    focusPipelineIdPath = &active_bands_band_2_focus_pipeline_id;
  } else {
    return ResultCode::ERROR_SETTING_BAND_ID_INVALID;
  }
  return getPipelineId(*focusPipelineIdPath, pPipelineId);
}

ResultCode
SimpleRadioSettings::isSplitBands(bool* pIsSplitBands)
{
  return getBool(active_bands_is_split, pIsSplitBands);
}
ResultCode
SimpleRadioSettings::getBandId(const SettingFieldPath& path, SplitBandId* pBandId)
{
  SettingFieldVariant focusBandId;
  ResultCode rc = getField(path, focusBandId);
  if (rc == ResultCode::OK) {
    auto* focusBandIdPtr = etl::get_if<uint32_t>(&focusBandId);
    if (focusBandIdPtr != nullptr) {
      *pBandId = static_cast<SplitBandId>(*focusBandIdPtr);
    } else {
      rc = ResultCode::ERROR_SETTING_BAND_ID_INVALID;
    }
  }
  return rc;
}

ResultCode
SimpleRadioSettings::getPipelineId(const SettingFieldPath& path, PipelineId* pPipelineId)
{
  SettingFieldVariant focusPipelineId;
  ResultCode rc = getField(path, focusPipelineId);
  if (rc == ResultCode::OK) {
    auto* focusPipelineIdPtr = etl::get_if<uint32_t>(&focusPipelineId);
    if (focusPipelineIdPtr != nullptr) {
      *pPipelineId = static_cast<PipelineId>(*focusPipelineIdPtr);
    } else {
      rc = ResultCode::ERROR_SETTING_PIPELINE_ID_INVALID;
    }
  }
  return rc;
}

ResultCode
SimpleRadioSettings::getBool(const SettingFieldPath& path, bool* pBool)
{
  SettingFieldVariant focusBool;
  ResultCode rc = getField(path, focusBool);
  if (rc == ResultCode::OK) {
    auto* focusBoolPtr = etl::get_if<bool>(&focusBool);
    if (focusBoolPtr != nullptr) {
      *pBool = *focusBoolPtr;
    } else {
      rc = ResultCode::ERROR_SETTING_BOOL_INVALID;
    }
  }
  return rc;
}