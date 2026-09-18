#pragma once

#include <ResultCode.h>
#include <settings/model/message/FieldDescriptor.h>
#include <settings/model/radios/IActiveBandSettingsT.h>
#include "BasicBandSettings.h"
#include "BasicBandSettingsCache.h"

class BasicActiveBandSettings : public IActiveBandSettingsT<BasicBandSettings>
{
public:
  using Proto = makesdr_BasicActiveBandSettingsPb;
  using BandSettings = BasicBandSettings;

  BasicActiveBandSettings(Proto& raw);

  [[nodiscard]] bool hasFocusBand() const override { return m_rawSettings.has_focus_band; }
  BandSettings* focusBandSettings() override;
  [[nodiscard]] const BandSettings* focusBandSettings() const override;

  [[nodiscard]] const Band * getFocusBand() const override { return m_bandSettings.getBand(); }
  [[nodiscard]] const Mode * getFocusMode() const override { return m_bandSettings.getFocusMode(); }

  ResultCode updateIndirectField(const FieldUpdate &settingUpdate, uint32_t startingAtIndex) override
  {
    return ResultCode::ERR_SETTING_INDIRECT_PATH_INVALID; // No indirect fields in "Basic" classes
  }

  ResultCode autoComplete(
    const BandCategoryList* bands,
    const ModeList* modes,
    BasicBandSettingsCache* cache
    );

  ResultCode autoComplete(
    const FieldDescriptor& setting,
    uint32_t startIndex,
    const BandCategoryList* bands,
    const ModeList* modes,
    BasicBandSettingsCache* cache
    );
protected:
  Proto& m_rawSettings;
  BasicBandSettings m_bandSettings;
};