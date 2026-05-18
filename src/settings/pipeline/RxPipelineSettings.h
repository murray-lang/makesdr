#pragma once
#include "AgcSpeed.h"
#include "IfSettings.h"
#include "RfSettings.h"
#include "../base/SettingsBase.h"


class RxPipelineSettings : public SettingsBase
{
public:
  RxPipelineSettings(RadioSettings_RxPipelineSettingsPb& rawSettings)
    : m_rawSettings(rawSettings)
    , m_mode(rawSettings.mode)
    , m_rfSettings(rawSettings.rf)
    , m_ifSettings(rawSettings.if_)
  {}

  [[nodiscard]] bool hasMode() const { return m_rawSettings.has_mode; }
  [[nodiscard]] bool hasRf() const { return m_rawSettings.has_rf; }
  [[nodiscard]] bool hasIf_() const { return m_rawSettings.has_if_; }
  [[nodiscard]] bool hasAgcSpeed() const { return m_rawSettings.has_agc_speed; }
  [[nodiscard]] bool hasMute() const { return m_rawSettings.has_mute; }

  // [[nodiscard]] Mode::Type mode() const { return static_cast<Mode::Type>(m_rawSettings.mode); }
  Mode& mode() { return m_mode; }

  RfSettings& rfSettings() { return m_rfSettings; }
  [[nodiscard]] const RfSettings& rfSettings() const { return m_rfSettings; }

  IfSettings& ifSettings() { return m_ifSettings; }
  [[nodiscard]] const IfSettings& ifSettings() const { return m_ifSettings; }

  [[nodiscard]] AgcSpeed agcSpeed() const { return static_cast<AgcSpeed>(m_rawSettings.agc_speed); }
  [[nodiscard]] bool isMute() const { return m_rawSettings.mute; }

protected:
  RadioSettings_RxPipelineSettingsPb& m_rawSettings;
  Mode m_mode;
  RfSettings m_rfSettings;
  IfSettings m_ifSettings;
};