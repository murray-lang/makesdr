#pragma once
#include "BandSettingsCache.h"
#include "band/BandSettings.h"
#include "band/SplitBandId.h"
#include "../../base/writeable/AutoComplete.h"
#include "base/SettingFieldVariant.h"
#include "base/writeable/ResolveIndirection.h"

class WriteableBandSettings : public BandSettings, public ResolveIndirection, public AutoComplete
{
public:
  WriteableBandSettings(RadioSettings_BandSettingsPb& raw)
    : BandSettings(raw)
    , m_bandSettingsCache(nullptr)
  {}

  void setBandSettingsCache(BandSettingsCache* bandSettingsCache) { m_bandSettingsCache = bandSettingsCache; }

  ResultCode setBandRequest(NameString& bandName);

  ResultCode applyBandDefaults(const ModeList& modeInfo);

  ResultCode autoComplete(
    const SettingFieldPath& path,
    uint32_t startingAtIndex,
    AutoCompleteTrigger trigger) override;

  ResultCode resolveIndirection(
    const SettingFieldPath& indirectPath,
    uint32_t startingAtIndex,
    SettingFieldPath& resolvedPath
  ) override;

  ResultCode autoCompleteBandRequest();

protected:

  BandSettingsCache* m_bandSettingsCache;
};
