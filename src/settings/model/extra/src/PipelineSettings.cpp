#include "settings/model/extra/PipelineSettings.h"

PipelineSettings::PipelineSettings(makesdr_PipelineSettingsPb& rawSettings)
    : m_rawSettings(rawSettings)
    , m_rfSettings(rawSettings.rf)
{
  if (m_rawSettings.which_mode_or_request == makesdr_PipelineSettingsPb_mode_request_tag) {
    m_modeOrRequest.emplace<Mode::Type>(static_cast<Mode::Type>(m_rawSettings.mode_or_request.mode_request));
  } else if (m_rawSettings.which_mode_or_request == makesdr_PipelineSettingsPb_mode_tag) {
    m_modeOrRequest.emplace<Mode>(m_rawSettings.mode_or_request.mode);
  }
}

ResultCode
PipelineSettings::autoComplete(
    const SettingPath& path,
    uint32_t startingAtIndex,
    AutoCompleteTrigger trigger)
{
  if (trigger == AutoCompleteTrigger::MODE) {
    if (startingAtIndex >= path.size()) {
      return ResultCode::ERR_SETTING_AUTOCOMPLETE_PATH_INVALID;
    }
    if (path[startingAtIndex] == makesdr_PipelineSettingsPb_mode_request_tag) {
      return autoCompleteMode();
    }
  }
  return ResultCode::OK;
}

ResultCode
PipelineSettings::autoComplete()
{
  return autoCompleteMode();
}

ResultCode
PipelineSettings::autoCompleteMode()
{
  if (m_rawSettings.which_mode_or_request == makesdr_PipelineSettingsPb_mode_request_tag) {
    if (m_categories == nullptr) return ResultCode::ERR_SETTING_BAND_SETTINGS_NO_INFO;

    const ModeList& modeList =  m_categories->modes();
    const makesdr_ModePb* pMode = modeList.findModeByType(m_rawSettings.mode_or_request.mode_request);
    if (pMode == nullptr) return ResultCode::ERR_SETTING_AUTOCOMPLETE_MODE_NOT_FOUND;

    m_rawSettings.mode_or_request.mode = *pMode;
    m_rawSettings.which_mode_or_request = makesdr_PipelineSettingsPb_mode_tag;
  }
  return ResultCode::OK;
}

ResultCode
PipelineSettings::applyBandDefaults(const Band* pBand, const ModeList& modeInfo)
{
  if (pBand != nullptr) {
    Mode::Type defaultMode = pBand->defaultMode();
    const makesdr_ModePb* pNewMode = modeInfo.findModeByType(static_cast<makesdr_ModeType>(defaultMode));
    if (pNewMode == nullptr) {
      return ResultCode::ERR_SETTING_MODE_TYPE_UNAVAILABLE;
    }
    // m_modeOrRequest.emplace<Mode>(*pNewMode);
    m_rfSettings.applyBandDefaults(pBand);
  }
  return ResultCode::OK;
}
