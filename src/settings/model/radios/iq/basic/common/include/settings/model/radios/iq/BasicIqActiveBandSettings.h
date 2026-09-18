#pragma once

#include "BasicIqBandSettings.h"
#include <settings/model/radios/IActiveBandSettingsT.h>

#include "BasicIqBandSettingsCache.h"

class BasicIqActiveBandSettings : public IActiveBandSettingsT<BasicIqBandSettings>
{
public:
  using Proto = makesdr_BasicIqActiveBandSettingsPb;
  using BandSettings = BasicIqBandSettings;

  BasicIqActiveBandSettings(Proto& raw);

  [[nodiscard]] bool hasFocusBand() const override { return m_rawSettings.has_focus_band; }
  [[nodiscard]] BandSettings* focusBandSettings() override;
  [[nodiscard]] const BandSettings* focusBandSettings() const override
  {
    return const_cast<BasicIqActiveBandSettings*>(this)->focusBandSettings();
  }
  [[nodiscard]] const Band* getFocusBand() const override { return m_focusBand.getBand(); }
  [[nodiscard]] const Mode* getFocusMode() const override { return m_focusBand.getFocusMode(); }

  ResultCode updateIndirectField(const FieldUpdate &settingUpdate, uint32_t startingAtIndex) override
  {
    return ResultCode::ERR_SETTING_INDIRECT_PATH_INVALID; // No indirect fields in "Basic" classes
  }

  ResultCode autoComplete(const BandCategoryList* bands, const ModeList* modes, BasicIqBandSettingsCache* cache);
  ResultCode autoComplete(
    const FieldDescriptor& setting,
    uint32_t startIndex,
    const BandCategoryList* bands,
    const ModeList* modes,
    BasicIqBandSettingsCache* cache
    );

protected:
  Proto& m_rawSettings;
  BasicIqBandSettings m_focusBand;
};
