#pragma once
#include "../base/SettingsBase.h"
#include "BandSettings.h"
#include "SplitBandId.h"

class ActiveBandSettings : public SettingsBase
{
public:
  ActiveBandSettings(RadioSettings_ActiveBandSettingsPb& raw)
    : SettingsBase(&RadioSettings_ActiveBandSettingsPb_msg)
    , m_rawSettings(raw)
    , m_band1Settings(raw.band_1)
    , m_band2Settings(raw.band_2)
  {}

  [[nodiscard]] bool hasFocusBandId() const { return m_rawSettings.has_focus_band_id; }
  [[nodiscard]] bool hasTxBandId() const { return m_rawSettings.has_tx_band_id; }
  [[nodiscard]] bool hasRxBandId() const { return m_rawSettings.has_rx_band_id; }
  [[nodiscard]] bool hasBand1() const { return m_rawSettings.has_band_1; }
  [[nodiscard]] bool hasBand2() const { return m_rawSettings.has_band_2; }

  BandSettings* getBandSettings(SplitBandId bandId)
  {
    if (bandId == SplitBandId::One) return &m_band1Settings;
    if (bandId == SplitBandId::Two) return &m_band2Settings;
    return nullptr;
  }

  [[nodiscard]] const BandSettings* getBandSettings(SplitBandId bandId) const
  {
    return const_cast<ActiveBandSettings*>(this)->getBandSettings(bandId);
  }

  BandSettings* getFocusBandSettings() { return getBandSettings(static_cast<SplitBandId>(m_rawSettings.focus_band_id)); }
  BandSettings* getTxBandSettings() { return getBandSettings(static_cast<SplitBandId>(m_rawSettings.tx_band_id)); }
  BandSettings* getRxBandSettings() { return getBandSettings(static_cast<SplitBandId>(m_rawSettings.rx_band_id)); }

  [[nodiscard]] const BandSettings* getFocusBandSettings() const
  {
    return const_cast<ActiveBandSettings*>(this)->getFocusBandSettings();
  }
  [[nodiscard]] const BandSettings* getTxBandSettings() const
  {
    return const_cast<ActiveBandSettings*>(this)->getTxBandSettings();
  }
  [[nodiscard]] const BandSettings* getRxBandSettings() const
  {
    return const_cast<ActiveBandSettings*>(this)->getRxBandSettings();
  }

  BandSettings& band1Settings() { return m_band1Settings; }
  BandSettings& band2Settings() { return m_band2Settings; }
  [[nodiscard]] const BandSettings& band1Settings() const { return m_band1Settings; }
  [[nodiscard]] const BandSettings& band2Settings() const { return m_band2Settings; }

protected:
  void* getMessage() override { return &m_rawSettings; }

protected:
  RadioSettings_ActiveBandSettingsPb& m_rawSettings;
  BandSettings m_band1Settings;
  BandSettings m_band2Settings;
};
