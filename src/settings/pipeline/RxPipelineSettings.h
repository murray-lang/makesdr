#pragma once
#include "AgcSpeed.h"
#include "IfSettings.h"
#include "RfSettings.h"
#include "PipelineSettings.h"
#include <etl/variant.h>

class RxPipelineSettings : public PipelineSettings<RadioSettings_RxPipelineSettingsPb>
{
public:
  RxPipelineSettings(RadioSettings_RxPipelineSettingsPb& rawSettings)
    : PipelineSettings(rawSettings)
    , m_rawSettings(rawSettings)
    , m_ifSettings(rawSettings.if_)
  {
  }
  [[nodiscard]] bool hasIf_() const { return m_rawSettings.has_if_; }
  [[nodiscard]] bool hasAgcSpeed() const { return m_rawSettings.has_agc_speed; }
  [[nodiscard]] bool hasMute() const { return m_rawSettings.has_mute; }

  IfSettings& ifSettings() { return m_ifSettings; }
  [[nodiscard]] const IfSettings& ifSettings() const { return m_ifSettings; }

  [[nodiscard]] AgcSpeed agcSpeed() const { return static_cast<AgcSpeed>(m_rawSettings.agc_speed); }
  [[nodiscard]] bool isMute() const { return m_rawSettings.mute; }

protected:
  RadioSettings_RxPipelineSettingsPb& m_rawSettings;
  IfSettings m_ifSettings;
};