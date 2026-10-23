#pragma once

#include "SingleIqBandSettings.h"
#include <settings/model/radio/ActiveBandSettingsDefaultsT.h>

#include "SingleIqBandSettingsCache.h"

class SingleIqActiveBandSettings : public ActiveBandSettingsDefaultsT<SingleIqActiveBandSettings, SingleIqBandSettings>
{
public:
  using Proto = makesdr_SingleIqActiveBandSettingsPb;
  using BandSettings = SingleIqBandSettings;

  SingleIqActiveBandSettings(Proto& raw);

  [[nodiscard]] bool hasFocusBand() const { return m_rawSettings.has_focus_band; }
  [[nodiscard]] BandSettings* focusBandSettings();
  [[nodiscard]] const BandSettings* focusBandSettings() const
  {
    return const_cast<SingleIqActiveBandSettings*>(this)->focusBandSettings();
  }
  [[nodiscard]] const Band* getFocusBand() const { return m_focusBand.getBand(); }
  [[nodiscard]] const Mode* getFocusMode() const { return m_focusBand.getFocusMode(); }

  ResultCode updateIndirectField(const FieldUpdate &settingUpdate, uint32_t startingAtIndex)
  {
    return ResultCode::ERR_SETTING_INDIRECT_PATH_INVALID; // No indirect fields in "Basic" classes
  }

  ResultCode autoComplete(const BandCategoryList* bands, const ModeList* modes, SingleIqBandSettingsCache* cache);
  ResultCode autoComplete(
    const FieldUpdate& setting,
    uint32_t startIndex,
    const BandCategoryList* bands,
    const ModeList* modes,
    SingleIqBandSettingsCache* cache
    );

protected:
  Proto& m_rawSettings;
  SingleIqBandSettings m_focusBand;
};
