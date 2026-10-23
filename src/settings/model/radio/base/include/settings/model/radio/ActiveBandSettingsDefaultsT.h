#pragma once

#include "SplitBandId.h"

// Non-virtual fallbacks for active band settings classes that don't support
// split operation. DerivedT must provide hasFocusBand() and focusBandSettings();
// a class that supports split hides these fallbacks with its own members.
// This is not an interface: active band settings are only ever accessed
// through their concrete type.
template<typename DerivedT, typename BandSettingsT>
class ActiveBandSettingsDefaultsT
{
public:
  [[nodiscard]] bool hasBand(SplitBandId bandId) const
  {
    return bandId == SplitBandId::One ? self().hasFocusBand() : false;
  }
  BandSettingsT* band(SplitBandId bandId)
  {
    return bandId == SplitBandId::One ? self().focusBandSettings() : nullptr;
  }
  [[nodiscard]] const BandSettingsT* band(SplitBandId bandId) const
  {
    return bandId == SplitBandId::One ? self().focusBandSettings() : nullptr;
  }

  [[nodiscard]] bool hasTxBand() const { return false; }
  BandSettingsT* txBand() { return nullptr; }
  [[nodiscard]] const BandSettingsT* txBand() const { return nullptr; }

  [[nodiscard]] bool hasRxBand() const { return self().hasFocusBand(); }
  BandSettingsT* rxBand() { return self().focusBandSettings(); }
  [[nodiscard]] const BandSettingsT* rxBand() const { return self().focusBandSettings(); }

  [[nodiscard]] bool hasIsSplit() const { return false; }
  [[nodiscard]] bool isSplit() const { return false; }

private:
  DerivedT& self() { return static_cast<DerivedT&>(*this); }
  [[nodiscard]] const DerivedT& self() const { return static_cast<const DerivedT&>(*this); }
};
