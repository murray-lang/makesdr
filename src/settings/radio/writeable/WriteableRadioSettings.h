#pragma once

#include "../../base/SettingsBase.h"
#include "../../band/writeable/WriteableActiveBandSettings.h"
#include "../ReceiverSettings.h"
#include "../RadioSettings.h"
#include "../TransmitterSettings.h"
#include "../../base/writeable/SettingsManipulator.h"
#include "../../util/DefaultRadioSettings.h"
#include "../../band/writeable/BandSettingsCache.h"
#include "base/writeable/AutoComplete.h"
#include "base/writeable/ResolveIndirection.h"

class WriteableRadioSettings :
  public RadioSettings<WriteableActiveBandSettings>,
  public ResolveIndirection,
  public AutoComplete
{
public:
  WriteableRadioSettings(const BandCategoryList& bandInfo,  const ModeList& modeInfo)
    : m_manipulator(&m_rawSettings, &RadioSettings_RadioSettingsPb_msg)
    , m_bandSettingsCache(bandInfo, modeInfo)
  {
  }

  WriteableRadioSettings(RadioSettings_RadioSettingsPb& raw, const BandCategoryList& bandInfo,  const ModeList& modeInfo)
  : RadioSettings(raw)
  , m_manipulator(&m_rawSettings, &RadioSettings_RadioSettingsPb_msg)
  , m_bandSettingsCache(bandInfo, modeInfo)
  {}

  static ResultCode resolveDottedPath(
    const char *dottedPath,
    SettingFieldPath &path,
    bool* isIndirectOut,
    AutoCompleteTrigger* triggerOut
  );

  ResultCode resolveIndirection(
    const SettingFieldPath& indirectPath,
    uint32_t startingAtIndex,
    SettingFieldPath& resolvedPath
  ) override;

  ResultCode applyUpdate(const SettingFieldUpdate& update);
  ResultCode autoComplete(const SettingFieldPath& path, uint32_t startingAtIndex, AutoCompleteTrigger trigger) override;

protected:
  SettingsManipulator m_manipulator;
  BandSettingsCache m_bandSettingsCache;
};
