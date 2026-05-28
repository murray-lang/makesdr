#pragma once
#include "model/basic/ActiveBandSettings.h"
#include "MutableBandSettings.h"
#include "AutoComplete.h"

class MutableActiveBandSettings :
  public ActiveBandSettings<MutableBandSettings>,
  public AutoComplete
{
public:
  MutableActiveBandSettings(RadioSettings_ActiveBandSettingsPb& raw)
    : ActiveBandSettings(raw)
  {
  }

  void setCategories(RadioCategories* categories)
  {
    m_bandOneSettings.setCategories(categories);
    m_bandTwoSettings.setCategories(categories);
  }

  ResultCode autoComplete(
    const SettingFieldPath& path,
    uint32_t startingAtIndex,
    AutoCompleteTrigger trigger) override;

  ResultCode autoComplete() override;

protected:
  ResultCode autoCompleteSplit();
};
