#pragma once
#include "SplitBandId.h"
#include "IBandSettingsView.h"

// Read-only, type-neutral view of the active bands, for the front end only.
// Mirrors the split-shaped API that ActiveBandSettingsDefaultsT gives every active band
// settings class: a radio without split has only band One, which always has focus.
//
// has*() reports presence, so on a view of changes it says what changed. The band selector
// values (focus/tx/rx band id, is split) are always effective: a change set only carries them
// when they changed, so otherwise they come from the state the changes were merged into.
class IActiveBandSettingsView
{
public:
  virtual ~IActiveBandSettingsView() = default;

  [[nodiscard]] virtual bool hasBand(SplitBandId bandId) const = 0;
  // Persistent for a view of the state; for a view of changes, only valid alongside it.
  // nullptr if the radio doesn't have that band.
  [[nodiscard]] virtual const IBandSettingsView* band(SplitBandId bandId) const = 0;

  [[nodiscard]] virtual bool hasFocusBandId() const = 0;
  [[nodiscard]] virtual SplitBandId focusBandId() const = 0;
  [[nodiscard]] virtual bool hasTxBandId() const = 0;
  // SplitBandId::None for a radio without TX
  [[nodiscard]] virtual SplitBandId txBandId() const = 0;
  [[nodiscard]] virtual bool hasRxBandId() const = 0;
  [[nodiscard]] virtual SplitBandId rxBandId() const = 0;
  [[nodiscard]] virtual bool hasIsSplit() const = 0;
  [[nodiscard]] virtual bool isSplit() const = 0;

  // The focus band, if present in these settings
  [[nodiscard]] const IBandSettingsView* focusBandSettings() const
  {
    const SplitBandId bandId = focusBandId();
    return hasBand(bandId) ? band(bandId) : nullptr;
  }
};
