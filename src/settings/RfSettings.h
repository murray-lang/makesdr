#pragma once
#include "base/SettingsBase.h"
#include "base/SteppableFloatSetting.h"
#include "steppable/SteppableInt64Setting.h"

class RfSettings : public SettingsBase
{
public:
  RfSettings(RadioSettings_RfSettingsPb& raw)
    : SettingsBase(&RadioSettings_RfSettingsPb_msg)
    , m_rawSettings(raw)
    , m_gain(raw.gain)
    , m_centreFrequency(raw.centre_frequency)
    , m_vfoFrequency(raw.vfo)
  {}

  [[nodiscard]] int32_t maxPositiveVfoOffset() const { return m_rawSettings.max_positive_vfo_offset; }
  [[nodiscard]] int32_t maxNegativeVfoOffset() const { return m_rawSettings.max_negative_vfo_offset; }

  SteppableFloatSetting& gain() { return m_gain; }
  SteppableInt64Setting& centreFrequency() { return m_centreFrequency; }
  SteppableInt64Setting& vfoFrequency() { return m_vfoFrequency; }

protected:
  RadioSettings_RfSettingsPb& m_rawSettings;
  SteppableFloatSetting m_gain;
  SteppableInt64Setting m_centreFrequency;
  SteppableInt64Setting m_vfoFrequency;
};
