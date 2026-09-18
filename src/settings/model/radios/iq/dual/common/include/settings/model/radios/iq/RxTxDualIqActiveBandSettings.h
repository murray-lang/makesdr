#pragma once

#include "RxTxDualIqBandSettings.h"
#include <settings/model/radios/IActiveBandSettingsT.h>

#include "DualIqBandSettingsCache.h"
#include "RxTxDualIqBandSettingsCache.h"
#include <settings/model/message/FieldDescriptor.h>

class RxTxDualIqActiveBandSettings : public IActiveBandSettingsT<RxTxDualIqBandSettings>
{
public:
  using Proto = makesdr_RxTxDualIqActiveBandSettingsPb;
  using BandSettings = RxTxDualIqBandSettings;

  RxTxDualIqActiveBandSettings(Proto& raw);

  [[nodiscard]] const Band* getFocusBand() const override { return m_focusBand.getBand(); }
  [[nodiscard]] const Mode* getFocusMode() const override { return m_focusBand.getFocusMode(); }
  [[nodiscard]] bool hasFocusBand() const override { return m_rawSettings.has_focus_band; }
  [[nodiscard]] BandSettings* focusBandSettings() override;
  [[nodiscard]] const BandSettings* focusBandSettings() const override
  {
    return const_cast<RxTxDualIqActiveBandSettings*>(this)->focusBandSettings();
  }

  ResultCode updateIndirectField(const FieldUpdate &settingUpdate, uint32_t startingAtIndex) override;

  ResultCode autoComplete(const BandCategoryList* bands, const ModeList* modes, RxTxDualIqBandSettingsCache* cache);
  ResultCode autoComplete(
    const FieldDescriptor& setting,
    uint32_t startIndex,
    const BandCategoryList* bands,
    const ModeList* modes,
    RxTxDualIqBandSettingsCache* cache
    );

protected:
  Proto& m_rawSettings;
  RxTxDualIqBandSettings m_focusBand;
};
