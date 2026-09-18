#include "settings/model/radios/iq/DualIqActiveBandSettings.h"

DualIqActiveBandSettings::DualIqActiveBandSettings(Proto& raw)
  : m_rawSettings(raw),
    m_focusBand(raw.focus_band)
{
}

DualIqActiveBandSettings::BandSettings*
DualIqActiveBandSettings::focusBandSettings()
{
  return &m_focusBand;
}

ResultCode
DualIqActiveBandSettings::updateIndirectField(const FieldUpdate &settingUpdate, uint32_t startingAtIndex)
{
  const FieldPath& path = settingUpdate.descriptor().getPath();
  if (startingAtIndex >= path.size()) {
    return ResultCode::ERR_SETTING_INDIRECT_PATH_INVALID;
  }
  SplitBandId bandId = SplitBandId::None;
  if (path[startingAtIndex] == makesdr_DualIqActiveBandSettingsPb_focus_band_tag) {
    return m_focusBand.updateIndirectField(settingUpdate, startingAtIndex + 1);
  }
  return ResultCode::ERR_SETTING_INDIRECT_PATH_INVALID;
}

ResultCode
DualIqActiveBandSettings::autoComplete(const BandCategoryList* bands, const ModeList* modes, DualIqBandSettingsCache* cache)
{
  return m_focusBand.autoComplete(bands, modes, cache);
}

ResultCode
DualIqActiveBandSettings::autoComplete(
  const FieldDescriptor& setting,
  uint32_t startIndex,
  const BandCategoryList* bands, const ModeList* modes,
  DualIqBandSettingsCache* cache
  )
{
  const FieldPath& path = setting.getPath();
  if (startIndex >= path.size()) {
    return ResultCode::ERR_SETTING_AUTOCOMPLETE_PATH_INVALID;
  }
  if (path[startIndex] == makesdr_DualIqActiveBandSettingsPb_focus_band_tag) {
    return m_focusBand.autoComplete(setting, startIndex + 1, bands, modes, cache);
  }
  return ResultCode::ERR_SETTING_AUTOCOMPLETE_NOT_IMPLEMENTED;
}

