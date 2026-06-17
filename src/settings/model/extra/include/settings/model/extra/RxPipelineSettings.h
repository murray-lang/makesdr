#pragma once
#include "AgcSpeed.h"
#include "IfSettings.h"
#include "PipelineSettings.h"

class RxPipelineSettings : public SettingsBase
{
public:
  RxPipelineSettings(makesdr_RxPipelineSettingsPb& rawSettings)
    : m_rawSettings(rawSettings)
    , m_base(rawSettings.base)
    , m_ifSettings(rawSettings.if_)
  {
  }
  PipelineSettings& base() { return m_base; }
  [[nodiscard]] const PipelineSettings& base() const { return m_base; }

  [[nodiscard]] bool hasIf_() const { return m_rawSettings.has_if_; }
  [[nodiscard]] bool hasAgcSpeed() const { return m_rawSettings.has_agc_speed; }
  [[nodiscard]] bool hasMute() const { return m_rawSettings.has_mute; }

  IfSettings& ifSettings() { return m_ifSettings; }
  [[nodiscard]] const IfSettings& ifSettings() const { return m_ifSettings; }

  [[nodiscard]] AgcSpeed agcSpeed() const { return static_cast<AgcSpeed>(m_rawSettings.agc_speed); }
  [[nodiscard]] bool isMute() const { return m_rawSettings.mute; }

protected:
  makesdr_RxPipelineSettingsPb& m_rawSettings;
  PipelineSettings m_base;
  IfSettings m_ifSettings;
};