#include "model/mutable/MutablePipelineSettings.h"
#include "model/mutable/BandSettingsCache.h"

ResultCode
MutablePipelineSettings::autoComplete(
    const SettingFieldPath& path,
    uint32_t startingAtIndex,
    AutoCompleteTrigger trigger)
{
  if (trigger == AutoCompleteTrigger::MODE) {
    if (startingAtIndex >= path.size()) {
      return ResultCode::ERR_SETTING_AUTOCOMPLETE_PATH_INVALID;
    }
    if (path[startingAtIndex] == RadioSettings_PipelineSettingsPb_mode_request_tag) {
      return autoCompleteMode();
    }
  }
  return ResultCode::OK;
}

ResultCode
MutablePipelineSettings::autoComplete()
{
  return autoCompleteMode();
}

ResultCode
MutablePipelineSettings::autoCompleteMode()
{
  if (m_rawSettings.which_mode_or_request == RadioSettings_PipelineSettingsPb_mode_request_tag) {
    if (m_categories == nullptr) return ResultCode::ERR_SETTING_BAND_SETTINGS_NO_INFO;

    const ModeList& modeList =  m_categories->modes();
    const Mode* pMode = modeList.findModeByType(static_cast<Mode::Type>(m_rawSettings.mode_or_request.mode_request));
    if (pMode == nullptr) return ResultCode::ERR_SETTING_AUTOCOMPLETE_MODE_NOT_FOUND;

    m_rawSettings.mode_or_request.mode = pMode->raw();
    m_rawSettings.which_mode_or_request = RadioSettings_PipelineSettingsPb_mode_tag;
  }
  return ResultCode::OK;
}

ResultCode
MutablePipelineSettings::applyBandDefaults(const Band* pBand, const ModeList& modeInfo)
{
  if (pBand != nullptr) {
    Mode::Type defaultMode = pBand->defaultMode();
    const Mode* pNewMode = modeInfo.findModeByType(defaultMode);
    if (pNewMode == nullptr) {
      return ResultCode::ERR_SETTING_MODE_TYPE_UNAVAILABLE;
    }
    m_modeOrRequest.emplace<Mode>(*pNewMode);
    m_rfSettings.applyBandDefaults(pBand);
  }
  return ResultCode::OK;
}