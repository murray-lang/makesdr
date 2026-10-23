#pragma once
#include <cstdint>
#include "RadioSettingsTraits.h"
#include "IActiveBandSettingsView.h"
#include "BandRfSettings.h"

// Read-only, type-neutral view of whichever RadioSettings flavour the radio reported.
// Intended for the front end only: the back end keeps using the concrete templated types,
// so no vtables are introduced on embedded targets.
// Use traits() to decide what to display; the views below answer for every flavour.
class IRadioSettingsView
{
public:
  virtual ~IRadioSettingsView() = default;

  [[nodiscard]] virtual RadioSettingsTraits traits() const = 0;

  // The accumulated settings this view is relative to. A view of the state returns itself;
  // a view of changes returns the state the changes were merged into. Use it for context
  // that a change set doesn't carry.
  [[nodiscard]] virtual const IRadioSettingsView* state() const = 0;

  [[nodiscard]] virtual bool hasActiveBands() const = 0;
  [[nodiscard]] virtual const IActiveBandSettingsView* activeBands() const = 0;

  [[nodiscard]] virtual bool hasPtt() const = 0;
  [[nodiscard]] virtual bool ptt() const = 0;

  // Focus band conveniences. nullptr / 0 when absent from these settings.
  [[nodiscard]] const IBandSettingsView* focusBandSettings() const
  {
    return hasActiveBands() ? activeBands()->focusBandSettings() : nullptr;
  }

  [[nodiscard]] const Band* getFocusBand() const
  {
    const IBandSettingsView* band = focusBandSettings();
    return band != nullptr ? band->getBand() : nullptr;
  }

  [[nodiscard]] const Mode* getFocusMode() const
  {
    const IBandSettingsView* band = focusBandSettings();
    return band != nullptr ? band->getFocusMode() : nullptr;
  }

  [[nodiscard]] const BandRfSettings* focusBandRf() const
  {
    const IBandSettingsView* band = focusBandSettings();
    return band != nullptr && band->hasRfSettings() ? band->rfSettings() : nullptr;
  }

  [[nodiscard]] int64_t getCentreFrequency() const
  {
    const BandRfSettings* rf = focusBandRf();
    return rf != nullptr ? rf->frequency() : 0;
  }

  [[nodiscard]] const RxPipelineSettings* focusPipeline() const
  {
    const IBandSettingsView* band = focusBandSettings();
    return band != nullptr && band->hasFocusPipeline() ? band->focusPipeline() : nullptr;
  }
};
