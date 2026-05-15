#pragma once
#include "RfSettings.h"
#include "../base/SettingsBase.h"


class TxPipelineSettings : public SettingsBase
{
public:
  TxPipelineSettings(RadioSettings_TxPipelineSettingsPb& rawSettings)
    : SettingsBase(&RadioSettings_TxPipelineSettingsPb_msg)
    , m_rawSettings(rawSettings)
    , m_rfSettings(rawSettings.rf)
  {}

  [[nodiscard]] bool hasMode() const { return m_rawSettings.has_mode; }
  [[nodiscard]] bool hasRf() const { return m_rawSettings.has_rf; }

  [[nodiscard]] Mode::Type mode() const { return static_cast<Mode::Type>(m_rawSettings.mode); }

  ResultCode setMode(Mode::Type modeType)
  {
    m_rawSettings.mode = static_cast<RadioSettings_ModeType>(modeType);
    m_rawSettings.has_mode = true;
    return ResultCode::OK;
  }

  RfSettings& rfSettings() { return m_rfSettings; }
  [[nodiscard]] const RfSettings& rfSettings() const { return m_rfSettings; }

protected:
  void* getMessage() override { return &m_rawSettings; }

  RadioSettings_TxPipelineSettingsPb& m_rawSettings;
  RfSettings m_rfSettings;
};