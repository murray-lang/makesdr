#pragma once
#include <CrossPlatformTypes.h>
#include "../../../../core/include/model/core/SettingsBase.h"
#include "RfSettings.h"
#include "Band.h"



using ModeOrRequestVariant = variant<monostate, Mode::Type, Mode>;

class PipelineSettings : SettingsBase
{
public:
  PipelineSettings(RadioSettings_PipelineSettingsPb& rawSettings);

  [[nodiscard]] bool isModeValid() const { return m_modeOrRequest.index() != 0; }
  [[nodiscard]] bool hasModeRequest() const { return m_modeOrRequest.index() == 1; }
  [[nodiscard]] bool hasMode() const { return m_modeOrRequest.index() == 2; }
  [[nodiscard]] bool hasRf() const { return m_rawSettings.has_rf; }

  Mode* mode() { return etl::get_if<Mode>(&m_modeOrRequest); }
  Mode::Type* modeRequest() { return etl::get_if<Mode::Type>(&m_modeOrRequest); }

  RfSettings& rfSettings() { return m_rfSettings; }
  [[nodiscard]] const RfSettings& rfSettings() const { return m_rfSettings; }

protected:
  RadioSettings_PipelineSettingsPb& m_rawSettings;
  ModeOrRequestVariant m_modeOrRequest;
  RfSettings m_rfSettings;
};