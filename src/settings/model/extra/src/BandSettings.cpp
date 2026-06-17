#include "settings/model/extra/BandSettings.h"

// #include "base/writeable/SettingsManipulator.h"


ResultCode
BandSettings::setBandRequest(NameString& bandName)
{
  m_rawSettings.which_band_or_request = makesdr_BandSettingsPb_band_request_tag;
  std::copy(bandName.c_str(), bandName.c_str() + bandName.length() + 1, m_rawSettings.band_or_request.band_request);
  setBandOrRequestVariant(m_rawSettings);
  return ResultCode::OK;
}

ResultCode
BandSettings::applyBandDefaults(const ModeList& modeInfo)
{
  const Band* pBand = band();
  if (pBand == nullptr) return ResultCode::ERR_SETTING_BAND_SETTINGS_HAS_NO_BAND;

  ResultCode rc = m_txPipeline.base().applyBandDefaults(pBand, modeInfo);
  if (rc != ResultCode::OK) return rc;
  rc = m_pipelineA.base().applyBandDefaults(pBand, modeInfo);
  if (rc != ResultCode::OK) return rc;
  rc = m_pipelineB.base().applyBandDefaults(pBand, modeInfo);

  return rc;
}

ResultCode
BandSettings::autoComplete(
    const SettingPath& path,
    uint32_t startingAtIndex,
    AutoCompleteTrigger trigger
)
{
  if (startingAtIndex >= path.size()) {
    return ResultCode::ERR_SETTING_AUTOCOMPLETE_PATH_INVALID;
  }
  switch (path[startingAtIndex]) {
    case makesdr_BandSettingsPb_band_request_tag:
        if (trigger == AutoCompleteTrigger::BAND) {
          return autoCompleteBandRequest();
        }
    break;
    case makesdr_BandSettingsPb_is_multi_pipeline_tag:
        if (trigger == AutoCompleteTrigger::MULTI_PIPELINE) {
          return autoCompleteMultiPipeline();
        }
    break;
    case makesdr_BandSettingsPb_pipeline_a_tag:
      return m_pipelineA.base().autoComplete(path, startingAtIndex + 1, trigger);
    break;
    case makesdr_BandSettingsPb_pipeline_b_tag:
      return m_pipelineB.base().autoComplete(path, startingAtIndex + 1, trigger);
    break;
    case makesdr_BandSettingsPb_tx_pipeline_tag:
      return m_txPipeline.base().autoComplete(path, startingAtIndex + 1, trigger);
    break;
  }
  return ResultCode::OK;
}

ResultCode
BandSettings::autoComplete()
{
  ResultCode rc = ResultCode::OK;

  rc = m_pipelineA.base().autoComplete();
  if (rc != ResultCode::OK) return rc;

  rc = m_pipelineB.base().autoComplete();
  if (rc != ResultCode::OK) return rc;

  rc = m_txPipeline.base().autoComplete();
  if (rc != ResultCode::OK) return rc;

  if (hasBandRequest()) {
    rc = autoCompleteBandRequest();
    if (rc != ResultCode::OK) return rc;
  }
  if (hasIsMultiPipeline()) {
    rc = autoCompleteMultiPipeline();
    if (rc != ResultCode::OK) return rc;
  }
  return rc;
}

ResultCode
BandSettings::autoCompleteBandRequest()
{

  if (m_rawSettings.which_band_or_request == makesdr_BandSettingsPb_band_request_tag) {
    if (m_categories == nullptr) return ResultCode::ERR_SETTING_BAND_SETTINGS_NO_INFO;

    const BandCategoryList& bandInfo = m_categories->bands();
    const makesdr_BandPb* pBand = bandInfo.findBand(m_rawSettings.band_or_request.band_request);
    if (pBand == nullptr) return ResultCode::ERR_SETTING_AUTOCOMPLETE_BAND_NOT_FOUND;

    m_rawSettings.band_or_request.band = *pBand;
    m_rawSettings.which_band_or_request = makesdr_BandSettingsPb_band_tag;

    return ResultCode::OK;
  }
  return ResultCode::OK;
}

ResultCode
BandSettings::autoCompleteMultiPipeline()
{
  if (isMultiPipeline()) {
    m_rawSettings.pipeline_b = m_rawSettings.pipeline_a;
    m_rawSettings.has_pipeline_b = true;
    m_rawSettings.tx_pipeline_id = makesdr_PipelineId_PIPELINE_B;
    m_rawSettings.has_tx_pipeline_id = true;
    copyBasicsForTxTracking(m_rawSettings.pipeline_b);
    m_rawSettings.focus_pipeline_id = makesdr_PipelineId_PIPELINE_B;
    m_rawSettings.has_focus_pipeline_id = true;
    return ResultCode::OK;
  } else {
    m_rawSettings.tx_pipeline_id = makesdr_PipelineId_PIPELINE_A;
    m_rawSettings.has_tx_pipeline_id = true;
    copyBasicsForTxTracking(m_rawSettings.pipeline_a);
    m_rawSettings.focus_pipeline_id = makesdr_PipelineId_PIPELINE_A;
    m_rawSettings.has_focus_pipeline_id = true;
  }
  return ResultCode::OK;
}

void
BandSettings::copyBasicsForTxTracking(makesdr_RxPipelineSettingsPb& rxPipeline)
{
  // SettingsManipulator manipulator(&m_rawSettings, &makesdr_BandSettingsPb_msg);
  m_rawSettings.tx_pipeline.base = rxPipeline.base;
}