#pragma once
#include "../ActiveBandSettings.h"
#include "AvailableBands.h"
#include "BandSettingsCache.h"
#include "WriteableBandSettings.h"
#include "../../base/writeable/SettingsManipulator.h"
#include "../../base/writeable/AutoComplete.h"
#include "base/writeable/ResolveIndirection.h"
#include "mode/AvailableModes.h"

class WriteableActiveBandSettings :
  public ActiveBandSettings<WriteableBandSettings>,
  public ResolveIndirection,
  public AutoComplete
{
public:
  WriteableActiveBandSettings(RadioSettings_ActiveBandSettingsPb& raw)
    : ActiveBandSettings(raw)
    , m_manipulator(&m_rawSettings, &RadioSettings_ActiveBandSettingsPb_msg)
    , m_bandInfo(availableBandsPb)
    , m_modeInfo(availableModesPb)
    , m_bandSettingsCache(m_bandInfo, m_modeInfo)
  {
    m_bandOneSettings.setBandSettingsCache(&m_bandSettingsCache);
    m_bandTwoSettings.setBandSettingsCache(&m_bandSettingsCache);
  }

  ResultCode autoComplete(
    const SettingFieldPath& path,
    uint32_t startingAtIndex,
    AutoCompleteTrigger trigger) override;

  ResultCode resolveIndirection(
    const SettingFieldPath& indirectPath,
    uint32_t startingAtIndex,
    SettingFieldPath& resolvedPath
  ) override;

protected:
  ResultCode autoCompleteSplit();

  SettingsManipulator m_manipulator;
  BandCategoryList m_bandInfo;
  ModeList m_modeInfo;
  BandSettingsCache m_bandSettingsCache;
};
