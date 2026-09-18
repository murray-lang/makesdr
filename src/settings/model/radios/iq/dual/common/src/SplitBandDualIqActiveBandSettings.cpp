#include "settings/model/radios/iq/SplitBandDualIqActiveBandSettings.h"

SplitBandDualIqActiveBandSettings::SplitBandDualIqActiveBandSettings(Proto& raw)
  : m_rawSettings(raw)
  , m_band_1(raw.band_1)
  , m_band_2(raw.band_2)
{
}

const Band*
SplitBandDualIqActiveBandSettings::getFocusBand() const
{
  if (const BandSettings* focus = focusBandSettings()) {
    return focus->getBand();
  }
  return nullptr;
}

const Mode*
SplitBandDualIqActiveBandSettings::getFocusMode() const
{
  if (const BandSettings* focus = focusBandSettings()) {
    return focus->getFocusMode();
  }
  return nullptr;
}

SplitBandDualIqActiveBandSettings::BandSettings*
SplitBandDualIqActiveBandSettings::focusBandSettings()
{
  SplitBandId bandId = focusBandId();
  return band(bandId);
}

SplitBandDualIqActiveBandSettings::BandSettings*
SplitBandDualIqActiveBandSettings::rxBand()
{
  if (hasRxBandId()) {
    SplitBandId bandId = rxBandId();
    return band(bandId);
  }
  return nullptr;
}

SplitBandDualIqActiveBandSettings::BandSettings*
SplitBandDualIqActiveBandSettings::txBand()
{
  if (hasTxBandId()) {
    SplitBandId bandId = txBandId();
    return band(bandId);
  }
  return nullptr;
}

bool
SplitBandDualIqActiveBandSettings::hasBand(SplitBandId bandId) const
{
  if (bandId == SplitBandId::One) {
    return m_rawSettings.has_band_1;
  }
  if (bandId == SplitBandId::Two) {
    return m_rawSettings.has_band_2;
  }
  return false;
}

SplitBandDualIqActiveBandSettings::BandSettings*
SplitBandDualIqActiveBandSettings::band(SplitBandId bandId)
{
  if (bandId == SplitBandId::One) {
    return &m_band_1;
  }
  if (bandId == SplitBandId::Two) {
    return &m_band_2;
  }
  return nullptr;
}

ResultCode
SplitBandDualIqActiveBandSettings::updateIndirectField(const FieldUpdate &settingUpdate, uint32_t startingAtIndex)
{
  const FieldPath& path = settingUpdate.descriptor().getPath();
  if (startingAtIndex >= path.size()) {
    return ResultCode::ERR_SETTING_INDIRECT_PATH_INVALID;
  }

  if (path[startingAtIndex] == makesdr_SplitBandDualIqActiveBandSettingsPb_focus_band_tag) {
    SplitBandId bandId = focusBandId();
    if (bandId == SplitBandId::None) {
      return ResultCode::ERR_SETTING_BAND_SETTINGS_FOCUS_BAND_NOT_SET;
    }
    if (bandId == SplitBandId::One ) {
      return m_band_1.updateIndirectField(settingUpdate, startingAtIndex + 1);
    }
    if (bandId == SplitBandId::Two) {
      return m_band_2.updateIndirectField(settingUpdate, startingAtIndex + 1);
    }
  }
  return ResultCode::ERR_SETTING_INDIRECT_PATH_INVALID;
}

ResultCode
SplitBandDualIqActiveBandSettings::autoComplete(
  const BandCategoryList* bands,
  const ModeList* modes,
  RxTxDualIqBandSettingsCache* cache)
{
  ResultCode rc1 = m_band_1.autoComplete(bands, modes, cache);
  ResultCode rc2 = m_band_2.autoComplete(bands, modes, cache);
  if (rc1 != ResultCode::OK) {
    return rc1;
  }
  if (rc2 != ResultCode::OK) {
    return rc2;
  }
  return ResultCode::OK;
}

ResultCode
SplitBandDualIqActiveBandSettings::autoComplete(
  const FieldDescriptor& setting,
  uint32_t startIndex,
  const BandCategoryList* bands,
  const ModeList* modes,
  RxTxDualIqBandSettingsCache* cache
  )
{
  const FieldPath& path = setting.getPath();

  if (startIndex >= path.size()) {
    return ResultCode::ERR_SETTING_AUTOCOMPLETE_PATH_INVALID;
  }
  uint32_t tag = path[startIndex];
  if (tag == makesdr_SplitBandDualIqActiveBandSettingsPb_band_1_tag) {
    return m_band_1.autoComplete(setting, startIndex + 1, bands, modes, cache);
  }
  if (tag == makesdr_SplitBandDualIqActiveBandSettingsPb_band_2_tag) {
    return m_band_2.autoComplete(setting, startIndex + 1, bands, modes, cache);
  }
  if (tag == makesdr_SplitBandDualIqActiveBandSettingsPb_focus_band_tag) {
    switch (focusBandId()) {
      case SplitBandId::One: return m_band_1.autoComplete(setting, startIndex + 1, bands, modes, cache);
      case SplitBandId::Two: return m_band_2.autoComplete(setting, startIndex + 1, bands, modes, cache);
      default: break;
    }
  }
  if (tag == makesdr_SplitBandDualIqActiveBandSettingsPb_is_split_tag) {
    return autoCompleteSplit(bands, modes, cache);
  }
  return ResultCode::ERR_SETTING_AUTOCOMPLETE_NOT_IMPLEMENTED;
}

ResultCode
SplitBandDualIqActiveBandSettings::autoCompleteSplit(
  const BandCategoryList* bands,
  const ModeList* modes,
  RxTxDualIqBandSettingsCache* cache
  )
{
  auto txId = static_cast<SplitBandId>(m_rawSettings.tx_band_id);
  if (m_rawSettings.is_split) {
    if (txId == SplitBandId::None) {
      if (m_rawSettings.band_2.which_band_or_request == 0) {
        m_rawSettings.tx_band_id = makesdr_SplitBandId_SPLIT_BAND_ONE;
        m_rawSettings.has_tx_band_id = true;
        return ResultCode::OK;
      }
      ResultCode rc = ResultCode::OK;
      if (m_rawSettings.band_2.which_band_or_request == makesdr_RxTxDualIqBandSettingsPb_band_request_tag) {
        rc = m_band_2.autoComplete(bands, modes, cache);
      } // Otherwise m_payload.body.band_2.which_band_or_request == makesdr_BandSettingsPb_band_tag
      if (rc == ResultCode::OK) {
        m_rawSettings.tx_band_id = makesdr_SplitBandId_SPLIT_BAND_TWO;
        m_rawSettings.has_tx_band_id = true;
      }
    }
  } else {
    auto focusId = static_cast<SplitBandId>(m_rawSettings.focus_band_id);
    if (txId != focusId) {
      m_rawSettings.tx_band_id = m_rawSettings.focus_band_id;
      m_rawSettings.has_tx_band_id = true;
    }
    if (m_rawSettings.rx_band_id != m_rawSettings.focus_band_id) {
      m_rawSettings.rx_band_id = m_rawSettings.focus_band_id;
      m_rawSettings.has_rx_band_id = true;
    }
  }
  return ResultCode::OK;
}
