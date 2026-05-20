#include "WriteableBandSettings.h"


ResultCode
WriteableBandSettings::setBandRequest(NameString& bandName)
{
  m_rawSettings.which_band_or_request = RadioSettings_BandSettingsPb_band_request_tag;
  std::copy(bandName.c_str(), bandName.c_str() + bandName.length() + 1, m_rawSettings.band_or_request.band_request);
  setBandOrRequestVariant(m_rawSettings);
  return ResultCode::OK;
}

ResultCode
WriteableBandSettings::applyBandDefaults(const ModeList& modeInfo)
{
  const Band* pBand = band();
  if (pBand == nullptr) return ResultCode::ERR_SETTING_BAND_SETTINGS_HAS_NO_BAND;

  ResultCode rc = m_txPipeline.applyBandDefaults(pBand, modeInfo);
  if (rc != ResultCode::OK) return rc;
  rc = m_pipelineA.applyBandDefaults(pBand, modeInfo);
  if (rc != ResultCode::OK) return rc;
  rc = m_pipelineB.applyBandDefaults(pBand, modeInfo);

  return rc;
}

ResultCode
WriteableBandSettings::autoComplete(
    const SettingFieldPath& path,
    uint32_t startingAtIndex,
    AutoCompleteTrigger trigger
)
{
  if (startingAtIndex >= path.size()) {
    return ResultCode::ERR_SETTING_AUTOCOMPLETE_PATH_INVALID;
  }
  switch (path[startingAtIndex]) {
    case RadioSettings_BandSettingsPb_band_request_tag:
        if (trigger == AutoCompleteTrigger::BAND) {
          return autoCompleteBandRequest();
        }
    // case RadioSettings_BandSettingsPb_band_tag:
    // case RadioSettings_BandSettingsPb_pipeline_a_tag:
    // case RadioSettings_BandSettingsPb_pipeline_b_tag:
    // case RadioSettings_BandSettingsPb_tx_pipeline_tag:
    // case RadioSettings_BandSettingsPb_is_multi_pipeline_tag:
    break;
  }
  return ResultCode::OK;
}

ResultCode
WriteableBandSettings::resolveIndirection(
  const SettingFieldPath& indirectPath,
  uint32_t startingAtIndex,
  SettingFieldPath& resolvedPath
  )
{
  if (startingAtIndex >= indirectPath.size()) {
    return ResultCode::ERR_SETTING_RESOLVE_INDIRECTION_PATH_INVALID;
  }
  uint32_t nextIndex = startingAtIndex;
  switch (indirectPath[startingAtIndex]) {
  case RadioSettings_BandSettingsPb_focus_pipeline_tag:
    {
      PipelineId pipelineId = focusPipelineId();
      if (pipelineId == PipelineId::A) {
        resolvedPath.emplace_back(RadioSettings_BandSettingsPb_pipeline_a_tag);
      } else if (pipelineId == PipelineId::B) {
        resolvedPath.emplace_back(RadioSettings_BandSettingsPb_pipeline_b_tag);
      } else {
        return ResultCode::ERR_SETTING_BAND_SETTINGS_FOCUS_PIPELINE_NOT_SET;
      }
      nextIndex++;
      return ResultCode::OK;
    }
    break;
  default: break;
  }
  // We happen to know that pipelines have no indirect paths (for now). Just copy the rest.
  for (uint32_t i = nextIndex; i < indirectPath.size(); i++) {
    resolvedPath.emplace_back(indirectPath[i]);
  }
  return ResultCode::OK;

}

ResultCode
WriteableBandSettings::autoCompleteBandRequest()
{
  if (m_bandSettingsCache == nullptr) return ResultCode::ERR_SETTING_BAND_SETTINGS_NO_INFO;

  return m_bandSettingsCache->get(this);
}