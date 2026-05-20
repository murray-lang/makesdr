#pragma once
#include "../base/SettingsBase.h"
#include "BandSettings.h"
#include "SplitBandId.h"

template <typename BandSettingsType>
class ActiveBandSettings : public SettingsBase
{
public:
  ActiveBandSettings(RadioSettings_ActiveBandSettingsPb& raw)
    : m_rawSettings(raw)
    , m_bandOneSettings(raw.band_1)
    , m_bandTwoSettings(raw.band_2)
  {}

  [[nodiscard]] bool isSplit() const { return m_rawSettings.is_split; }
  [[nodiscard]] bool hasFocusBandId() const { return m_rawSettings.has_focus_band_id; }
  [[nodiscard]] bool hasTxBandId() const { return m_rawSettings.has_tx_band_id; }
  [[nodiscard]] bool hasRxBandId() const { return m_rawSettings.has_rx_band_id; }
  [[nodiscard]] bool hasBand1() const { return m_rawSettings.has_band_1; }
  [[nodiscard]] bool hasBand2() const { return m_rawSettings.has_band_2; }

  [[nodiscard]] SplitBandId focusBandId() const { return static_cast<SplitBandId>(m_rawSettings.focus_band_id); }
  [[nodiscard]] SplitBandId txBandId() const { return static_cast<SplitBandId>(m_rawSettings.tx_band_id); }
  [[nodiscard]] SplitBandId rxBandId() const { return static_cast<SplitBandId>(m_rawSettings.rx_band_id); }

  BandSettingsType* getBandSettings(SplitBandId bandId)
  {
    if (bandId == SplitBandId::One) return &m_bandOneSettings;
    if (bandId == SplitBandId::Two) return &m_bandTwoSettings;
    return nullptr;
  }

  [[nodiscard]] const BandSettingsType* getBandSettings(SplitBandId bandId) const
  {
    return const_cast<ActiveBandSettings*>(this)->getBandSettings(bandId);
  }

  BandSettingsType* getFocusBandSettings() { return getBandSettings(static_cast<SplitBandId>(m_rawSettings.focus_band_id)); }
  BandSettingsType* getTxBandSettings() { return getBandSettings(static_cast<SplitBandId>(m_rawSettings.tx_band_id)); }
  BandSettingsType* getRxBandSettings() { return getBandSettings(static_cast<SplitBandId>(m_rawSettings.rx_band_id)); }

  [[nodiscard]] const BandSettingsType* getFocusBandSettings() const
  {
    return const_cast<ActiveBandSettings*>(this)->getFocusBandSettings();
  }
  [[nodiscard]] const BandSettingsType* getTxBandSettings() const
  {
    return const_cast<ActiveBandSettings*>(this)->getTxBandSettings();
  }
  [[nodiscard]] const BandSettingsType* getRxBandSettings() const
  {
    return const_cast<ActiveBandSettings*>(this)->getRxBandSettings();
  }

  BandSettingsType& bandOneSettings() { return m_bandOneSettings; }
  BandSettingsType& bandTwoSettings() { return m_bandTwoSettings; }
  [[nodiscard]] const BandSettings& bandOneSettings() const { return m_bandOneSettings; }
  [[nodiscard]] const BandSettings& bandTwoSettings() const { return m_bandTwoSettings; }

protected:
  RadioSettings_ActiveBandSettingsPb& m_rawSettings;
  BandSettingsType m_bandOneSettings;
  BandSettingsType m_bandTwoSettings;

};
