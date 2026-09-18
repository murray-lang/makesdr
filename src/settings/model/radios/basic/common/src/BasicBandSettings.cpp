#include "settings/model/radios/basic/BasicBandSettings.h"

BasicBandSettings::BasicBandSettings(makesdr_BasicBandSettingsPb& rawSettings)
  : WithBandT(rawSettings)
  , WithModeT(rawSettings)
  , m_rawSettings(rawSettings)
  , m_rfSettings(rawSettings.rf)
  , m_ifSettings(rawSettings.if_)
{
}

ResultCode
BasicBandSettings::autoComplete(const BandCategoryList* bands, const ModeList* modes, BasicBandSettingsCache* cache)
{
  ResultCode rcBand = autoCompleteBand(bands, modes, cache, this);
  ResultCode rcMode = autoCompleteMode(modes);
  if (rcBand != ResultCode::OK) {
    return rcBand;
  }
  if (rcMode != ResultCode::OK) {
    return rcMode;
  }
  return ResultCode::OK;
}

ResultCode
BasicBandSettings::autoComplete(
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
  if (path[startIndex] == makesdr_BasicBandSettingsPb_band_request_tag) {
    return autoCompleteBand(setting, startIndex, bands, modes, cache, this);
  } else if (path[startIndex] == makesdr_BasicBandSettingsPb_mode_request_tag) {
    return autoCompleteMode(setting, startIndex, modes);
  }
  return ResultCode::ERR_SETTING_AUTOCOMPLETE_NOT_IMPLEMENTED;
}

ResultCode
BasicBandSettings::applyBandDefaults(const Band& band, const BandCategoryList* bands, const ModeList* modes)
{
  setModeRequest(band.defaultMode());
  ResultCode rc = autoCompleteMode(modes);
  if (rc != ResultCode::OK) return rc;
  return m_rfSettings.applyBandDefaults(band, bands, modes);
}

