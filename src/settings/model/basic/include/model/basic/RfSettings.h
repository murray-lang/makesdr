#pragma once
#include "../../../../core/include/model/core/SettingsBase.h"
#include "Band.h"
#include "SteppableFloatSetting.h"
#include "SteppableInt64Setting.h"

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

  void setMaxPositiveVfoOffset(int32_t offset) {
    m_rawSettings.max_positive_vfo_offset = offset;
    m_rawSettings.has_max_positive_vfo_offset = true;
  }
  void setMaxNegativeVfoOffset(int32_t offset) {
    m_rawSettings.max_negative_vfo_offset = offset;
    m_rawSettings.has_max_negative_vfo_offset = true;
  }

  SteppableFloatSetting& gain() { return m_gain; }
  [[nodiscard]] const SteppableFloatSetting& gain() const { return m_gain; }

  SteppableInt64Setting& centreFrequency() { return m_centreFrequency; }
  [[nodiscard]] const SteppableInt64Setting& centreFrequency() const { return m_centreFrequency; }

  SteppableInt64Setting& vfoFrequency() { return m_vfoFrequency; }
  [[nodiscard]] const SteppableInt64Setting& vfoFrequency() const { return m_vfoFrequency; }

  void applyBandDefaults(const Band* pBand)
  {
      uint64_t freqPlusOffset = m_centreFrequency.value() + m_vfoFrequency.value();
      if (!hasCentreFrequency() || !hasVfoFrequency() || !pBand->containsFrequency(freqPlusOffset)) {
        m_rawSettings.has_centre_frequency = true;
        m_rawSettings.has_vfo = true;
        m_rawSettings.centre_frequency.value = pBand->landingFrequency();
        m_rawSettings.centre_frequency.coarse_delta = pBand->defaultCoarseStep();
        m_rawSettings.centre_frequency.fine_delta = pBand->defaultFineStep();
        m_rawSettings.vfo.value = pBand->landingFrequency();
        m_rawSettings.vfo.coarse_delta = pBand->defaultCoarseStep();
        m_rawSettings.vfo.fine_delta = pBand->defaultFineStep();
      }
  }

protected:
  RadioSettings_RfSettingsPb& m_rawSettings;
  SteppableFloatSetting m_gain;
  SteppableInt64Setting m_centreFrequency;
  SteppableInt64Setting m_vfoFrequency;
};
