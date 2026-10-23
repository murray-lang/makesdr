#pragma once

#include "RxTxDualIqBandSettings.h"
#include <settings/model/radio/ActiveBandSettingsDefaultsT.h>

#include "DualIqBandSettingsCache.h"
#include "RxTxDualIqBandSettingsCache.h"
#include <settings/model/update/FieldDescriptor.h>

class RxTxDualIqActiveBandSettings : public ActiveBandSettingsDefaultsT<RxTxDualIqActiveBandSettings, RxTxDualIqBandSettings>
{
public:
  using Proto = makesdr_RxTxDualIqActiveBandSettingsPb;
  using BandSettings = RxTxDualIqBandSettings;

  RxTxDualIqActiveBandSettings(Proto& raw);

  [[nodiscard]] const Band* getFocusBand() const { return m_focusBand.getBand(); }
  [[nodiscard]] const Mode* getFocusMode() const { return m_focusBand.getFocusMode(); }
  [[nodiscard]] bool hasFocusBand() const { return m_rawSettings.has_focus_band; }
  [[nodiscard]] BandSettings* focusBandSettings();
  [[nodiscard]] const BandSettings* focusBandSettings() const
  {
    return const_cast<RxTxDualIqActiveBandSettings*>(this)->focusBandSettings();
  }

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
  Proto& m_rawSettings;
  RxTxDualIqBandSettings m_focusBand;
};
