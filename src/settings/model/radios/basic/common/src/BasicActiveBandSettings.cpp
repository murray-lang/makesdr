#include <settings/model/radios/IBandSettings.h>
#include <settings/model/message/FieldDescriptor.h>
#include <settings/model/radios/basic/BasicActiveBandSettings.h>
#include <settings/model/proto/RadioSettings.pb.h>

BasicActiveBandSettings::BasicActiveBandSettings(makesdr_BasicActiveBandSettingsPb& raw)
  : m_rawSettings(raw),
    m_bandSettings(raw.focus_band)
{
}

BasicActiveBandSettings::BandSettings*
BasicActiveBandSettings::focusBandSettings()
{
  if (m_rawSettings.has_focus_band) {
    return &m_bandSettings;
  }
  return nullptr;
}

const BasicActiveBandSettings::BandSettings*
BasicActiveBandSettings::focusBandSettings() const
{
  return const_cast<BasicActiveBandSettings*>(this)->focusBandSettings();
}


ResultCode
BasicActiveBandSettings::autoComplete(
  const BandCategoryList* bands, const ModeList* modes, BasicBandSettingsCache* cache)
{
  return m_bandSettings.autoComplete(bands, modes, cache);
}

ResultCode
BasicActiveBandSettings::autoComplete(
  const FieldDescriptor& setting,
  uint32_t startIndex,
  const BandCategoryList* bands,
  const ModeList* modes,
  BasicBandSettingsCache* cache
  )
{
  const FieldPath& path = setting.getPath();
  if (startIndex >= path.size()) {
    return ResultCode::ERR_SETTING_AUTOCOMPLETE_PATH_INVALID;
  }
  if (path[startIndex] == makesdr_BasicActiveBandSettingsPb_focus_band_tag) {
    return m_bandSettings.autoComplete(setting, startIndex + 1, bands, modes, cache);
  }
  return ResultCode::ERR_SETTING_AUTOCOMPLETE_NOT_IMPLEMENTED;
}
