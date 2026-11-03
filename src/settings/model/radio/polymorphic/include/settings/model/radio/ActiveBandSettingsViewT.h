#pragma once
#include "IActiveBandSettingsView.h"
#include "BandSettingsViewT.h"

// Adapts a concrete active band settings class to IActiveBandSettingsView.
// Header-only and only instantiated by the front end, via RadioSettingsViewT.
template<typename ActiveBandSettingsT>
class ActiveBandSettingsViewT final : public IActiveBandSettingsView
{
public:
  using BandSettings = typename ActiveBandSettingsT::BandSettings;

  // pState: the state these active bands are changes to, or nullptr if they are the state.
  // hasTx: whether the radio transmits, for the band selector values of non-split radios.
  ActiveBandSettingsViewT(
    const ActiveBandSettingsT& activeBands,
    const IActiveBandSettingsView* pState,
    bool hasTx
    )
    : m_activeBands(activeBands)
    , m_pState(pState)
    , m_hasTx(hasTx)
    , m_band1(activeBands.band(SplitBandId::One))
    , m_band2(activeBands.band(SplitBandId::Two))
  {}
  ActiveBandSettingsViewT(const ActiveBandSettingsViewT&) = delete;
  ActiveBandSettingsViewT& operator=(const ActiveBandSettingsViewT&) = delete;

  [[nodiscard]] bool hasBand(SplitBandId bandId) const override { return m_activeBands.hasBand(bandId); }

  [[nodiscard]] const IBandSettingsView* band(SplitBandId bandId) const override
  {
    if (bandId == SplitBandId::One && m_band1.isValid()) return &m_band1;
    if (bandId == SplitBandId::Two && m_band2.isValid()) return &m_band2;
    return nullptr;
  }

  [[nodiscard]] bool hasFocusBandId() const override
  {
    if constexpr (hasBandSelector) return m_activeBands.hasFocusBandId();
    else return false;
  }

  [[nodiscard]] SplitBandId focusBandId() const override
  {
    if constexpr (hasBandSelector) {
      return hasFocusBandId() || m_pState == nullptr ? m_activeBands.focusBandId() : m_pState->focusBandId();
    } else {
      return SplitBandId::One;
    }
  }

  [[nodiscard]] bool hasTxBandId() const override
  {
    if constexpr (hasBandSelector) return m_activeBands.hasTxBandId();
    else return false;
  }

  [[nodiscard]] SplitBandId txBandId() const override
  {
    if constexpr (hasBandSelector) {
      return hasTxBandId() || m_pState == nullptr ? m_activeBands.txBandId() : m_pState->txBandId();
    } else {
      return m_hasTx ? SplitBandId::One : SplitBandId::None;
    }
  }

  [[nodiscard]] bool hasRxBandId() const override
  {
    if constexpr (hasBandSelector) return m_activeBands.hasRxBandId();
    else return false;
  }

  [[nodiscard]] SplitBandId rxBandId() const override
  {
    if constexpr (hasBandSelector) {
      return hasRxBandId() || m_pState == nullptr ? m_activeBands.rxBandId() : m_pState->rxBandId();
    } else {
      return SplitBandId::One;
    }
  }

  // ActiveBandSettingsDefaultsT answers these for non-split radios.
  [[nodiscard]] bool hasIsSplit() const override { return m_activeBands.hasIsSplit(); }

  [[nodiscard]] bool isSplit() const override
  {
    return hasIsSplit() || m_pState == nullptr ? m_activeBands.isSplit() : m_pState->isSplit();
  }

private:
  static constexpr bool hasBandSelector = requires(const ActiveBandSettingsT& activeBands) {
    activeBands.hasFocusBandId();
    activeBands.focusBandId();
    activeBands.hasTxBandId();
    activeBands.txBandId();
    activeBands.hasRxBandId();
    activeBands.rxBandId();
  };

  const ActiveBandSettingsT& m_activeBands;
  const IActiveBandSettingsView* m_pState;
  bool m_hasTx;
  BandSettingsViewT<BandSettings> m_band1;
  BandSettingsViewT<BandSettings> m_band2;
};
