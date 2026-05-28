#include "model/basic/PipelineSettings.h"

PipelineSettings::PipelineSettings(RadioSettings_PipelineSettingsPb& rawSettings)
    : m_rawSettings(rawSettings)
    , m_rfSettings(rawSettings.rf)
{
  if (m_rawSettings.which_mode_or_request == RadioSettings_PipelineSettingsPb_mode_request_tag) {
    m_modeOrRequest.emplace<Mode::Type>(static_cast<Mode::Type>(m_rawSettings.mode_or_request.mode_request));
  } else if (m_rawSettings.which_mode_or_request == RadioSettings_PipelineSettingsPb_mode_tag) {
    m_modeOrRequest.emplace<Mode>(m_rawSettings.mode_or_request.mode);
  }
}
