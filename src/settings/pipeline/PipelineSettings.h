#pragma once
#include "base/SettingsBase.h"
#include "RfSettings.h"
#include "band/Band.h"
#include "mode/ModeList.h"
#include <etl/variant.h>



using ModeOrRequestVariant = etl::variant<etl::monostate, Mode::Type, Mode>;

template <typename MessageType>
class PipelineSettings : SettingsBase
{
public:
  PipelineSettings(MessageType& rawSettings)
    : m_rawSettings(rawSettings)
    , m_rfSettings(rawSettings.rf)
  {
    if (m_rawSettings.which_mode_or_request == RadioSettings_RxPipelineSettingsPb_mode_request_tag) {
      m_modeOrRequest.emplace<Mode::Type>(static_cast<Mode::Type>(m_rawSettings.mode_or_request.mode_request));
    } else if (m_rawSettings.which_mode_or_request == RadioSettings_RxPipelineSettingsPb_mode_tag) {
      m_modeOrRequest.emplace<Mode>(m_rawSettings.mode_or_request.mode);
    }
  }

  [[nodiscard]] bool isModeValid() const { return m_modeOrRequest.index() != 0; }
  [[nodiscard]] bool hasModeRequest() const { return m_modeOrRequest.index() == 1; }
  [[nodiscard]] bool hasMode() const { return m_modeOrRequest.index() == 2; }
  [[nodiscard]] bool hasRf() const { return m_rawSettings.has_rf; }

  Mode* mode() { return etl::get_if<Mode>(&m_modeOrRequest); }
  Mode::Type* modeRequest() { return etl::get_if<Mode::Type>(&m_modeOrRequest); }

  RfSettings& rfSettings() { return m_rfSettings; }
  [[nodiscard]] const RfSettings& rfSettings() const { return m_rfSettings; }

  ResultCode applyBandDefaults(const Band* pBand, const ModeList& modeInfo)
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

protected:
  MessageType& m_rawSettings;
  ModeOrRequestVariant m_modeOrRequest;
  RfSettings m_rfSettings;
};