#pragma once

#include "DualIqBandSettings.h"
#include "DualIqBandSettingsCache.h"
#include <settings/model/radios/IActiveBandSettingsT.h>

#include <settings/model/message/FieldDescriptor.h>

class DualIqActiveBandSettings : public IActiveBandSettingsT<DualIqBandSettings>
{
public:
  using Proto = makesdr_DualIqActiveBandSettingsPb;
  using BandSettings = DualIqBandSettings;

  DualIqActiveBandSettings(Proto& raw);

  [[nodiscard]] const Band* getFocusBand() const override { return m_focusBand.getBand(); }
  [[nodiscard]] const Mode* getFocusMode() const override { return m_focusBand.getFocusMode(); }
  [[nodiscard]] bool hasFocusBand() const override { return m_rawSettings.has_focus_band; }
  [[nodiscard]] BandSettings* focusBandSettings() override;
  [[nodiscard]] const BandSettings* focusBandSettings() const override
  {
    return const_cast<DualIqActiveBandSettings*>(this)->focusBandSettings();
  }

  ResultCode updateIndirectField(const FieldUpdate &settingUpdate, uint32_t startingAtIndex) override;

  ResultCode autoComplete(const BandCategoryList* bands, const ModeList* modes, DualIqBandSettingsCache* cache);
  ResultCode autoComplete(
    const FieldDescriptor& setting,
    uint32_t startIndex,
    const BandCategoryList* bands,
    const ModeList* modes,
    DualIqBandSettingsCache* cache
    );

protected:
  Proto& m_rawSettings;
  DualIqBandSettings m_focusBand;
};
