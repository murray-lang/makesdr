#pragma once
#include "AgcSpeed.h"
#include "IfSettings.h"

template <typename PipelineSettingsBase>
class RxPipelineSettings : public SettingsBase
{
public:
  RxPipelineSettings(RadioSettings_RxPipelineSettingsPb& rawSettings)
    : m_rawSettings(rawSettings)
    , m_base(rawSettings.base)
    , m_ifSettings(rawSettings.if_)
  {
  }
  PipelineSettingsBase& base() { return m_base; }
  const PipelineSettingsBase& base() const { return m_base; }

  [[nodiscard]] bool hasIf_() const { return m_rawSettings.has_if_; }
  [[nodiscard]] bool hasAgcSpeed() const { return m_rawSettings.has_agc_speed; }
  [[nodiscard]] bool hasMute() const { return m_rawSettings.has_mute; }

  IfSettings& ifSettings() { return m_ifSettings; }
  [[nodiscard]] const IfSettings& ifSettings() const { return m_ifSettings; }

  [[nodiscard]] AgcSpeed agcSpeed() const { return static_cast<AgcSpeed>(m_rawSettings.agc_speed); }
  [[nodiscard]] bool isMute() const { return m_rawSettings.mute; }

protected:
  RadioSettings_RxPipelineSettingsPb& m_rawSettings;
  PipelineSettingsBase m_base;
  IfSettings m_ifSettings;
};