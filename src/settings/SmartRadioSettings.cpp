#include "SmartRadioSettings.h"
#include "util/ResolvedFieldPaths.h"
#include "mode/ModeList.h"
#include "band/AvailableBands.h"
#include "band/BandCategoryList.h"

ResultCode
SmartRadioSettings::setFocusPipelineMode(SplitBandId bandId, const Mode::Type& mode)
{
  PipelineId focusPipelineId;
  ResultCode rc = getFocusPipelineId(bandId, &focusPipelineId);
  if (rc == ResultCode::OK) {
    rc = setMode(bandId, focusPipelineId, mode);
  }
  return rc;
}

ResultCode
SmartRadioSettings::setFocusMode(const Mode::Type& mode)
{
  SplitBandId focusBandId;
  ResultCode rc = getFocusBandId(&focusBandId);
  if (rc == ResultCode::OK) {
    rc = setFocusPipelineMode(focusBandId, mode);
  }
  return rc;
}

ResultCode
SmartRadioSettings::setFocusBand(const char* bandName)
{
  SplitBandId focusBandId;
  ResultCode rc = getFocusBandId(&focusBandId);
  if (rc == ResultCode::OK) {
    if (focusBandId == SplitBandId::None) {
      // If it's not set then set it now.
      focusBandId = SplitBandId::One;
      rc = updateField(active_bands_focus_band_id, static_cast<uint32_t>(focusBandId));
    }
    if (rc == ResultCode::OK) {
      rc = setBand(focusBandId, bandName);
    }
  }
  return rc;
}

ResultCode
SmartRadioSettings::splitBands(bool split, const NameString& newBandName)
{
  bool isSplit = false;
  ResultCode rc = isSplitBands(&isSplit);
  if (rc != ResultCode::OK) return rc;

  if (isSplit == split) {
    return isSplit ? ResultCode::ERROR_SETTING_BANDS_ALREADY_SPLIT : ResultCode::ERROR_SETTING_BANDS_ALREADY_UNSPLIT;
  }

  // This will be needed either way
  SplitBandId txBandId;
  rc = getTxBandId(&txBandId);
  if (rc != ResultCode::OK) return rc;

  if (split) {
    SettingFieldUpdate band2NameUpdate(active_bands_band_2_name, newBandName);
    rc = updateField(band2NameUpdate);
    if (rc != ResultCode::OK) return rc;

    // If tx_band_id is not set, then set it to band 1 (not 2. Doesn't really matter.)
    if (txBandId == SplitBandId::None) {
      SettingFieldUpdate txBandIdUpdate(active_bands_tx_band_id, static_cast<uint32_t>(SplitBandId::One));
      rc = updateField(txBandIdUpdate);
      if (rc != ResultCode::OK) return rc;
    }
  } else {
    // If we're not split, then all the band indicators must be band 1
    // We already have the tx band id
    if (txBandId != SplitBandId::One) {
      SettingFieldUpdate bandIdUpdate(active_bands_tx_band_id, static_cast<uint32_t>(SplitBandId::One));
      rc = updateField(bandIdUpdate);
      if (rc != ResultCode::OK) return rc;
    }
    // Now the focus band
    SplitBandId focusBandId;
    rc = getFocusBandId(&focusBandId);
    if (rc != ResultCode::OK) return rc;

    if (focusBandId != SplitBandId::One) {
      SettingFieldUpdate bandIdUpdate(active_bands_focus_band_id, static_cast<uint32_t>(SplitBandId::One));
      rc = updateField(bandIdUpdate);
      if (rc != ResultCode::OK) return rc;
    }
    // now the rx band
    SplitBandId rxBandId;
    rc = getRxBandId(&rxBandId);
    if (rc != ResultCode::OK) return rc;

    if (rxBandId != SplitBandId::One) {
      SettingFieldUpdate focusBandIdUpdate(active_bands_rx_band_id, static_cast<uint32_t>(SplitBandId::One));
      rc = updateField(focusBandIdUpdate);
      if (rc != ResultCode::OK) return rc;
    }
  }
  // Finally, update the split flag itself
  SettingFieldUpdate update(active_bands_is_split, split);
  return updateField(update);
}



ResultCode
SmartRadioSettings::setBand(SplitBandId bandId, const char* bandName)
{
  const SettingFieldPath* bandIdPath = nullptr;
  if (bandId == SplitBandId::One) {
    bandIdPath = &active_bands_band_1_name;
  } else if (bandId == SplitBandId::Two) {
    bandIdPath = &active_bands_band_2_name;
  } else {
    return ResultCode::ERROR_SETTING_BAND_ID_INVALID;
  }
  SettingFieldUpdate update(*bandIdPath, bandName);
  return updateField(update);
}



ResultCode
SmartRadioSettings::setMode(SplitBandId bandId, PipelineId pipelineId, const Mode::Type& mode)
{
  const SettingFieldPath* specifiedModePath = nullptr;
  const SettingFieldPath* txPipelineIdPath = nullptr;
  const SettingFieldPath* txPipelineModePath = nullptr;

  if (bandId == SplitBandId::One) {
    txPipelineIdPath = &active_bands_band_1_tx_pipeline_id;
    txPipelineModePath = &active_bands_band_1_tx_pipeline_mode;
    if (pipelineId == PipelineId::A) {
      specifiedModePath = &active_bands_band_1_pipeline_a_mode;

    } else if (pipelineId == PipelineId::B) {
      specifiedModePath = &active_bands_band_1_pipeline_b_mode;
    } else {
      return ResultCode::ERROR_SETTING_PIPELINE_ID_INVALID;
    }
  } else if (bandId == SplitBandId::Two) {
    txPipelineIdPath = &active_bands_band_2_tx_pipeline_id;
    txPipelineModePath = &active_bands_band_1_tx_pipeline_mode;
    if (pipelineId == PipelineId::A) {
      specifiedModePath = &active_bands_band_2_pipeline_a_mode;
    } else if (pipelineId == PipelineId::B) {
      specifiedModePath = &active_bands_band_2_pipeline_b_mode;
    } else {
      return ResultCode::ERROR_SETTING_PIPELINE_ID_INVALID;
    }
  } else {
    return ResultCode::ERROR_SETTING_BAND_ID_INVALID;
  }
  SettingFieldUpdate specifiedUpdate(*specifiedModePath, static_cast<uint32_t>(mode));
  ResultCode rc = updateField(specifiedUpdate);

  if (rc == ResultCode::OK) {
    // Now see if the tx pipeline follows the given rx pipeline.
    // If so then update it too.
    SettingFieldVariant txPipelineId;
    rc = getField(*txPipelineIdPath, txPipelineId);
    if (rc == ResultCode::OK) {
      auto* txPipelineIdPtr = etl::get_if<uint32_t>(&txPipelineId);
      if (txPipelineIdPtr != nullptr) {
        if (*txPipelineIdPtr == static_cast<uint32_t>(pipelineId)) {
          SettingFieldUpdate txUpdate(*txPipelineModePath, static_cast<uint32_t>(mode));
          rc = updateField(txUpdate);
        }
      } else {
        rc = ResultCode::ERROR_SETTING_PIPELINE_ID_INVALID;
      }
    }
  }
  return rc;
}

