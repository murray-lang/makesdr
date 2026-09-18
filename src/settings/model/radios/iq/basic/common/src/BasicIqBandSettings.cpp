#include "settings/model/radios/iq/BasicIqBandSettings.h"

BasicIqBandSettings::BasicIqBandSettings(Proto& rawSettings)
  : WithBandT(rawSettings)
  , m_rawSettings(rawSettings)
  , m_rfSettings(rawSettings.rf)
  , m_ifSettings(rawSettings.if_)
  , m_focusPipeline(rawSettings.focus_pipeline)
{
}

ResultCode
BasicIqBandSettings::autoComplete(const BandCategoryList* bands, const ModeList* modes, BasicIqBandSettingsCache* cache)
{
  ResultCode rcBand = autoCompleteBand(bands, modes, cache, this);
  ResultCode rcPipeline = m_focusPipeline.autoComplete(modes);
  if (rcPipeline != ResultCode::OK) {
    return rcPipeline;
  }
  if (rcBand != ResultCode::OK) {
    return rcBand;
  }
  return ResultCode::OK;
}

ResultCode
BasicIqBandSettings::autoComplete(
  const FieldDescriptor& setting,
  uint32_t startIndex,
  const BandCategoryList* bands,
  const ModeList* modes,
  BasicIqBandSettingsCache* cache
  )
{
  const FieldPath& path = setting.getPath();
  if (startIndex >= path.size()) {
    return ResultCode::ERR_SETTING_AUTOCOMPLETE_PATH_INVALID;
  }
  if (path[startIndex] == makesdr_BasicIqBandSettingsPb_band_request_tag) {
    return autoCompleteBand(setting, startIndex + 1, bands, modes, cache, this);
  }
  if (path[startIndex] == makesdr_BasicIqBandSettingsPb_focus_pipeline_tag) {
    return m_focusPipeline.autoComplete(setting, startIndex + 1, modes);
  }
  return ResultCode::ERR_SETTING_AUTOCOMPLETE_NOT_IMPLEMENTED;
}

ResultCode
BasicIqBandSettings::applyBandDefaults(const Band& band, const BandCategoryList* bands, const ModeList* modes)
{
  ResultCode rc = m_rfSettings.applyBandDefaults(band, bands, modes);
  if (rc != ResultCode::OK)  return rc;
  return m_focusPipeline.base().applyBandDefaults(band, bands, modes);
}
