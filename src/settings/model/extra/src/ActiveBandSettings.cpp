#include "settings/model/extra/ActiveBandSettings.h"


ResultCode
ActiveBandSettings::autoComplete(
  const SettingPath& path,
  uint32_t startingAtIndex,
  AutoCompleteTrigger trigger
)
{
  if (startingAtIndex >= path.size()) {
    return ResultCode::ERR_SETTING_AUTOCOMPLETE_PATH_INVALID;
  }
  switch (path[startingAtIndex]) {
  case RadioSettings_ActiveBandSettingsPb_band_1_tag:
    return m_bandOneSettings.autoComplete(path, startingAtIndex + 1, trigger);
  case RadioSettings_ActiveBandSettingsPb_band_2_tag:
    return m_bandTwoSettings.autoComplete(path, startingAtIndex + 1, trigger);
  case RadioSettings_ActiveBandSettingsPb_is_split_tag:
    if (trigger == AutoCompleteTrigger::SPLIT_BAND) {
      return autoCompleteSplit();
    }

  default: break;
  }
  return ResultCode::ERR_SETTING_AUTOCOMPLETE_NOT_IMPLEMENTED;
}

ResultCode
ActiveBandSettings::autoComplete()
{
  ResultCode rc = m_bandOneSettings.autoComplete();
  if (rc != ResultCode::OK) return rc;

  rc = m_bandTwoSettings.autoComplete();
  if (rc != ResultCode::OK) return rc;

  if (hasIsSplit()) {
    return autoCompleteSplit();
  }
  return ResultCode::OK;
}

ResultCode
ActiveBandSettings::autoCompleteSplit()
{
  SplitBandId txId = txBandId();
  if (isSplit()) {
    if (txId == SplitBandId::None) {
      if (m_rawSettings.band_2.which_band_or_request == 0) {
        m_rawSettings.tx_band_id = RadioSettings_SplitBandId_SPLIT_BAND_ONE;
        m_rawSettings.has_tx_band_id = true;
        return ResultCode::OK;
      }
      ResultCode rc = ResultCode::OK;
      if (m_rawSettings.band_2.which_band_or_request == RadioSettings_BandSettingsPb_band_request_tag) {
        rc = m_bandTwoSettings.autoCompleteBandRequest();
      } // Otherwise m_rawSettings.band_2.which_band_or_request == RadioSettings_BandSettingsPb_band_tag
      if (rc == ResultCode::OK) {
        m_rawSettings.tx_band_id = RadioSettings_SplitBandId_SPLIT_BAND_TWO;
        m_rawSettings.has_tx_band_id = true;
      }
    }
  } else {
    if (txId != SplitBandId::One) {
      m_rawSettings.tx_band_id = RadioSettings_SplitBandId_SPLIT_BAND_ONE;
      m_rawSettings.has_tx_band_id = true;
    }
    if (focusBandId() != SplitBandId::One) {
      m_rawSettings.focus_band_id = RadioSettings_SplitBandId_SPLIT_BAND_ONE;
      m_rawSettings.has_focus_band_id = true;
    }
    if (rxBandId() != SplitBandId::One) {
      m_rawSettings.rx_band_id = RadioSettings_SplitBandId_SPLIT_BAND_ONE;
      m_rawSettings.has_rx_band_id = true;
    }
  }
  return ResultCode::OK;
}
