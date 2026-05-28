#pragma once
#include "PipelineSettings.h"

template <typename PipelineSettingsBase>
class TxPipelineSettings : public SettingsBase
{
public:
  TxPipelineSettings(RadioSettings_TxPipelineSettingsPb& rawSettings)
    : m_base(rawSettings.base)
    , m_rawSettings(rawSettings)
  {}

  PipelineSettingsBase& base() { return m_base; }
  const PipelineSettingsBase& base() const { return m_base; }

protected:
  RadioSettings_TxPipelineSettingsPb& m_rawSettings;
  PipelineSettingsBase m_base;
};