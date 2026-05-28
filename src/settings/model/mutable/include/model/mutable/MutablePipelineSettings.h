#pragma once
#include "AutoComplete.h"
#include "model/basic/PipelineSettings.h"
#include "RadioCategories.h"

class BandSettingsCache;

class MutablePipelineSettings : public PipelineSettings, public AutoComplete
{
public:
  MutablePipelineSettings(RadioSettings_PipelineSettingsPb& rawSettings)
    : PipelineSettings(rawSettings)
    , m_categories(nullptr)
  {
  }

  void setCategories(RadioCategories* categories) { m_categories = categories; }

  ResultCode autoComplete(
    const SettingFieldPath& path,
    uint32_t startingAtIndex,
    AutoCompleteTrigger trigger) override;

  ResultCode autoComplete() override;

  ResultCode autoCompleteMode();

  ResultCode applyBandDefaults(const Band* pBand, const ModeList& modeInfo);

protected:
  RadioCategories* m_categories;
};