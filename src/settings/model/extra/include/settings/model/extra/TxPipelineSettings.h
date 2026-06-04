#pragma once
#include "PipelineSettings.h"

class TxPipelineSettings : public SettingsBase
{
public:
  TxPipelineSettings(RadioSettings_TxPipelineSettingsPb& rawSettings)
    : m_base(rawSettings.base)
    , m_rawSettings(rawSettings)
  {}

  PipelineSettings& base() { return m_base; }
  const PipelineSettings& base() const { return m_base; }

protected:
  RadioSettings_TxPipelineSettingsPb& m_rawSettings;
  PipelineSettings m_base;
};