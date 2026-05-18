#pragma once
#include "PipelineSettings.h"


class TxPipelineSettings : public PipelineSettings<RadioSettings_TxPipelineSettingsPb>
{
public:
  TxPipelineSettings(RadioSettings_TxPipelineSettingsPb& rawSettings)
    : PipelineSettings(rawSettings)
    , m_rawSettings(rawSettings)
  {}

protected:
  RadioSettings_TxPipelineSettingsPb& m_rawSettings;
};