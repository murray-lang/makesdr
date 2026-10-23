#include "settings/model/radio/iq/SingleIqBandSettings.h"

SingleIqBandSettings::SingleIqBandSettings(Proto& rawSettings)
  : WithBandT(rawSettings)
  , m_rawSettings(rawSettings)
  , m_rfSettings(rawSettings.rf)
  , m_ifSettings(rawSettings.if_)
  , m_focusPipeline(rawSettings.focus_pipeline)
{
}

ResultCode
SingleIqBandSettings::autoComplete(const BandCategoryList* bands, const ModeList* modes, SingleIqBandSettingsCache* cache)
{
  ResultCode rcBand = autoCompleteBand(nullptr, bands, modes, cache, this);
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
SingleIqBandSettings::autoComplete(
  const FieldUpdate& setting,
  uint32_t startIndex,
  const BandCategoryList* bands,
  const ModeList* modes,
  SingleIqBandSettingsCache* cache
  )
{
  const FieldPath& path = setting.path();
  if (startIndex >= path.size()) {
    return ResultCode::ERR_SETTING_AUTOCOMPLETE_PATH_INVALID;
  }
  if (path[startIndex] == makesdr_SingleIqBandSettingsPb_band_request_tag) {
    return autoCompleteBand(setting, startIndex + 1, bands, modes, cache, this);
  }
  if (path[startIndex] == makesdr_SingleIqBandSettingsPb_focus_pipeline_tag) {
    ResultCode rc = m_focusPipeline.autoComplete(setting, startIndex + 1, modes);
    if (rc == ResultCode::OK) {
      m_rawSettings.has_focus_pipeline = true;
    }
    return rc;
  }
  return ResultCode::ERR_SETTING_AUTOCOMPLETE_NOT_IMPLEMENTED;
}

ResultCode
SingleIqBandSettings::applyBandDefaults(const Band& band, const BandCategoryList* bands, const ModeList* modes)
{
  ResultCode rc = m_rfSettings.applyBandDefaults(band, bands, modes);
  if (rc != ResultCode::OK)  return rc;
  m_rawSettings.has_rf = true;
  rc = m_focusPipeline.base().applyBandDefaults(band, bands, modes);
  if (rc == ResultCode::OK) {
    m_rawSettings.has_focus_pipeline = true;
  }
  return rc;
}
