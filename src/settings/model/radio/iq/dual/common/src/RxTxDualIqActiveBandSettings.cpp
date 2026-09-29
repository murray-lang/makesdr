#include "settings/model/radio/iq/RxTxDualIqActiveBandSettings.h"

RxTxDualIqActiveBandSettings::RxTxDualIqActiveBandSettings(Proto& raw)
  : m_rawSettings(raw),
    m_focusBand(raw.focus_band)
{
}

RxTxDualIqActiveBandSettings::BandSettings*
RxTxDualIqActiveBandSettings::focusBandSettings()
{
  return &m_focusBand;
}

ResultCode
RxTxDualIqActiveBandSettings::updateIndirectField(const FieldUpdate &settingUpdate, uint32_t startingAtIndex)
{
  const FieldPath& path = settingUpdate.descriptor().getPath();
  if (startingAtIndex >= path.size()) {
    return ResultCode::ERR_SETTING_INDIRECT_PATH_INVALID;
  }
  if (path[startingAtIndex] == makesdr_RxTxDualIqActiveBandSettingsPb_focus_band_tag) {
    ResultCode rc = m_focusBand.updateIndirectField(settingUpdate, startingAtIndex + 1);
    if (rc == ResultCode::OK) {
      this->m_rawSettings.has_focus_band = true;
    }
  }
  return ResultCode::ERR_SETTING_INDIRECT_PATH_INVALID;
}

ResultCode
RxTxDualIqActiveBandSettings::autoComplete(const BandCategoryList* bands, const ModeList* modes, RxTxDualIqBandSettingsCache* cache)
{
  ResultCode rc =  m_focusBand.autoComplete(bands, modes, cache);
  if (rc == ResultCode::OK) {
    this->m_rawSettings.has_focus_band = true;
  }
  return rc;
}

ResultCode
RxTxDualIqActiveBandSettings::autoComplete(
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
  if (path[startIndex] == makesdr_RxTxDualIqActiveBandSettingsPb_focus_band_tag) {
    ResultCode rc = m_focusBand.autoComplete(setting, startIndex + 1, bands, modes, cache);
    if (rc == ResultCode::OK) {
      this->m_rawSettings.has_focus_band = true;
    }
  }
  return ResultCode::ERR_SETTING_AUTOCOMPLETE_NOT_IMPLEMENTED;
}

