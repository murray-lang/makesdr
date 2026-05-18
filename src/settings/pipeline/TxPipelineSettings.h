#pragma once
#include "RfSettings.h"
#include "../base/SettingsBase.h"


class TxPipelineSettings : public SettingsBase
{
public:
  TxPipelineSettings(RadioSettings_TxPipelineSettingsPb& rawSettings)
    : m_rawSettings(rawSettings)
    , m_mode(rawSettings.mode)
    , m_rfSettings(rawSettings.rf)
  {}

  [[nodiscard]] bool hasMode() const { return m_rawSettings.has_mode; }
  [[nodiscard]] bool hasRf() const { return m_rawSettings.has_rf; }

  // [[nodiscard]] Mode::Type mode() const { return static_cast<Mode::Type>(m_rawSettings.mode); }
  Mode& mode() { return m_mode; }

  RfSettings& rfSettings() { return m_rfSettings; }
  [[nodiscard]] const RfSettings& rfSettings() const { return m_rfSettings; }

protected:
  RadioSettings_TxPipelineSettingsPb& m_rawSettings;
  Mode m_mode;
  RfSettings m_rfSettings;
};