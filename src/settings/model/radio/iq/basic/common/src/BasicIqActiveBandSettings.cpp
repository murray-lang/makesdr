#include "settings/model/radio/iq/BasicIqActiveBandSettings.h"

BasicIqActiveBandSettings::BasicIqActiveBandSettings(Proto& raw)
  : m_rawSettings(raw),
    m_focusBand(raw.focus_band)
{
}

BasicIqActiveBandSettings::BandSettings*
BasicIqActiveBandSettings::focusBandSettings()
{
  if (m_rawSettings.has_focus_band) {
    return &m_focusBand;
  }
  return nullptr;
}

ResultCode
BasicIqActiveBandSettings::autoComplete(const BandCategoryList* bands, const ModeList* modes,  BasicIqBandSettingsCache* cache)
{
  return m_focusBand.autoComplete(bands, modes, cache);
}

ResultCode
BasicIqActiveBandSettings::autoComplete(
  const FieldUpdate& setting,
  uint32_t startIndex,
  const BandCategoryList* bands,
  const ModeList* modes,
  BasicIqBandSettingsCache* cache
  )
{
  const FieldPath& path = setting.path();
  if (startIndex >= path.size()) {
    return ResultCode::ERR_SETTING_AUTOCOMPLETE_PATH_INVALID;
  }
  if (path[startIndex] == makesdr_BasicIqActiveBandSettingsPb_focus_band_tag) {
    ResultCode rc = m_focusBand.autoComplete(setting, startIndex + 1, bands, modes, cache);
    if (rc == ResultCode::OK) {
      m_rawSettings.has_focus_band = true;
    }
    return rc;
  }
  return ResultCode::ERR_SETTING_AUTOCOMPLETE_NOT_IMPLEMENTED;
}
