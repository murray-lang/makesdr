#pragma once

#include <settings/model/radio/iq/RxTxDualIqBandSettings.h>
#include <settings/model/radio/SplitBandId.h>
#include <settings/model/radio/ActiveBandSettingsDefaultsT.h>

#include <settings/model/update/FieldDescriptor.h>

class SplitBandDualIqActiveBandSettings : public ActiveBandSettingsDefaultsT<SplitBandDualIqActiveBandSettings, RxTxDualIqBandSettings>
{
public:
  using Proto = makesdr_SplitBandDualIqActiveBandSettingsPb;
  using BandSettings = RxTxDualIqBandSettings;

  SplitBandDualIqActiveBandSettings(Proto& raw);

  // [[nodiscard]] bool hasBand1() const { return m_rawSettings.has_band_1; }
  // [[nodiscard]] const RxTxDualIqBandSettings& band1() const { return m_band_1; }
  // [[nodiscard]] bool hasBand2() const { return m_rawSettings.has_band_2; }
  // [[nodiscard]] const RxTxDualIqBandSettings& band2() const { return m_band_2; }

  [[nodiscard]] const Band* getFocusBand() const;
  [[nodiscard]] const Mode* getFocusMode() const;

  [[nodiscard]] bool hasFocusBandId() const { return m_rawSettings.has_focus_band_id; }
  [[nodiscard]] SplitBandId focusBandId() const { return static_cast<SplitBandId>(m_rawSettings.focus_band_id); }

  [[nodiscard]] bool hasFocusBand() const  { return hasBand(focusBandId()); }
  [[nodiscard]] BandSettings* focusBandSettings();
  [[nodiscard]] const BandSettings* focusBandSettings() const
  {
    return const_cast<SplitBandDualIqActiveBandSettings*>(this)->focusBandSettings();
  }

  [[nodiscard]] bool hasBand(SplitBandId bandId) const;
  BandSettings* band(SplitBandId bandId);
  [[nodiscard]] const BandSettings* band(SplitBandId bandId) const
  {
    return const_cast<SplitBandDualIqActiveBandSettings*>(this)->band(bandId);
  }

  [[nodiscard]] bool hasTxBandId() const { return m_rawSettings.has_tx_band_id; }
  [[nodiscard]] SplitBandId txBandId() const { return static_cast<SplitBandId>(m_rawSettings.tx_band_id); }

  [[nodiscard]] bool hasRxBandId() const { return m_rawSettings.has_rx_band_id; }
  [[nodiscard]] SplitBandId rxBandId() const { return static_cast<SplitBandId>(m_rawSettings.rx_band_id); }

  BandSettings* rxBand();
  [[nodiscard]] const BandSettings* rxBand() const
  {
    return const_cast<SplitBandDualIqActiveBandSettings*>(this)->rxBand();
  }

  BandSettings* txBand();
  [[nodiscard]] const BandSettings* txBand() const
  {
    return const_cast<SplitBandDualIqActiveBandSettings*>(this)->txBand();
  }

  [[nodiscard]] bool hasIsSplit() const { return m_rawSettings.has_is_split; }
  [[nodiscard]] bool isSplit() const { return m_rawSettings.is_split; }

  ResultCode updateIndirectField(const FieldUpdate &settingUpdate, uint32_t startingAtIndex);

  ResultCode autoComplete(const BandCategoryList* bands, const ModeList* modes, RxTxDualIqBandSettingsCache* cache);
  ResultCode autoComplete(
    const FieldUpdate& setting,
    uint32_t startIndex,
    const BandCategoryList* bands,
    const ModeList* modes,
    RxTxDualIqBandSettingsCache* cache
    );

protected:
  ResultCode autoCompleteSplit(
    const BandCategoryList* bands,
    const ModeList* modes,
    RxTxDualIqBandSettingsCache* cache
    );

  Proto& m_rawSettings;
  RxTxDualIqBandSettings m_band_1;
  RxTxDualIqBandSettings m_band_2;
};
