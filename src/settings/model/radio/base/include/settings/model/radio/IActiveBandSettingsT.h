#pragma once

#include "SplitBandId.h"
#include "IBandSettings.h"

template<typename BandSettingsT>
class IActiveBandSettingsT
{
public:
  using BandSettings = BandSettingsT;

  virtual ~IActiveBandSettingsT() = default;
  [[nodiscard]] virtual bool hasFocusBand() const = 0;
  virtual BandSettingsT* focusBandSettings() = 0;
  [[nodiscard]] virtual const BandSettingsT* focusBandSettings() const = 0;
  [[nodiscard]] virtual const Band* getFocusBand() const = 0;
  [[nodiscard]] virtual const Mode* getFocusMode() const = 0;

  [[nodiscard]] virtual bool hasBand(SplitBandId bandId) const
  {
    return bandId == SplitBandId::One ? hasFocusBand() : false;
  }
  virtual BandSettingsT* band(SplitBandId bandId)
  {
    return bandId == SplitBandId::One ? focusBandSettings() : nullptr;
  }
  [[nodiscard]] virtual const BandSettingsT* band(SplitBandId bandId) const
  {
    return bandId == SplitBandId::One ? focusBandSettings() : nullptr;
  }

  [[nodiscard]] virtual bool hasTxBand() const { return false; }
  virtual BandSettingsT* txBand() { return nullptr; }
  [[nodiscard]] virtual const BandSettingsT* txBand() const { return nullptr; }

  [[nodiscard]] virtual bool hasRxBand() const { return hasFocusBand(); }
  virtual BandSettingsT* rxBand() { return focusBandSettings(); }
  [[nodiscard]] virtual const BandSettingsT* rxBand() const { return focusBandSettings(); }

  [[nodiscard]] virtual bool hasIsSplit() const { return false; }
  [[nodiscard]] virtual bool isSplit() const { return false; }

  virtual ResultCode updateIndirectField(const FieldUpdate &settingUpdate, uint32_t startingAtIndex) = 0;
};
