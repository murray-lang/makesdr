#pragma once
#include "../base/SettingsBase.h"
#include "../base/SteppableFloatSetting.h"
#include "../base/SteppableInt64Setting.h"

class RfSettings : public SettingsBase
{
public:
  RfSettings(RadioSettings_RfSettingsPb& raw)
    : m_rawSettings(raw)
    , m_gain(raw.gain)
    , m_centreFrequency(raw.centre_frequency)
    , m_vfoFrequency(raw.vfo)
  {}

  [[nodiscard]] bool hasGain() const { return m_rawSettings.has_gain; }
  [[nodiscard]] bool hasCentreFrequency() const { return m_rawSettings.has_centre_frequency; }
  [[nodiscard]] bool hasVfoFrequency() const { return m_rawSettings.has_vfo; }

  [[nodiscard]] int32_t maxPositiveVfoOffset() const { return m_rawSettings.max_positive_vfo_offset; }
  [[nodiscard]] int32_t maxNegativeVfoOffset() const { return m_rawSettings.max_negative_vfo_offset; }

  SteppableFloatSetting& gain() { return m_gain; }
  [[nodiscard]] const SteppableFloatSetting& gain() const { return m_gain; }

  SteppableInt64Setting& centreFrequency() { return m_centreFrequency; }
  [[nodiscard]] const SteppableInt64Setting& centreFrequency() const { return m_centreFrequency; }

  SteppableInt64Setting& vfoFrequency() { return m_vfoFrequency; }
  [[nodiscard]] const SteppableInt64Setting& vfoFrequency() const { return m_vfoFrequency; }

protected:
  RadioSettings_RfSettingsPb& m_rawSettings;
  SteppableFloatSetting m_gain;
  SteppableInt64Setting m_centreFrequency;
  SteppableInt64Setting m_vfoFrequency;
};
