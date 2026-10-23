#pragma once

#include "DualIqBandSettings.h"
#include "DualIqBandSettingsCache.h"
#include <settings/model/radio/ActiveBandSettingsDefaultsT.h>

#include <settings/model/update/FieldDescriptor.h>

class DualIqActiveBandSettings : public ActiveBandSettingsDefaultsT<DualIqActiveBandSettings, DualIqBandSettings>
{
public:
  using Proto = makesdr_DualIqActiveBandSettingsPb;
  using BandSettings = DualIqBandSettings;

  DualIqActiveBandSettings(Proto& raw);

  [[nodiscard]] const Band* getFocusBand() const { return m_focusBand.getBand(); }
  [[nodiscard]] const Mode* getFocusMode() const { return m_focusBand.getFocusMode(); }
  [[nodiscard]] bool hasFocusBand() const { return m_rawSettings.has_focus_band; }
  [[nodiscard]] BandSettings* focusBandSettings();
  [[nodiscard]] const BandSettings* focusBandSettings() const
  {
    return const_cast<DualIqActiveBandSettings*>(this)->focusBandSettings();
  }

  ResultCode updateIndirectField(const FieldUpdate &settingUpdate, uint32_t startingAtIndex);

  ResultCode autoComplete(const BandCategoryList* bands, const ModeList* modes, DualIqBandSettingsCache* cache);
  ResultCode autoComplete(
    const FieldUpdate& setting,
    uint32_t startIndex,
    const BandCategoryList* bands,
    const ModeList* modes,
    DualIqBandSettingsCache* cache
    );

protected:
  Proto& m_rawSettings;
  DualIqBandSettings m_focusBand;
};
