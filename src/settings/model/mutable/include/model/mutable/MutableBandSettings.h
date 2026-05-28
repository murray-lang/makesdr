#pragma once
#include <model/basic/BandSettings.h>
#include <model/basic/SplitBandId.h>
#include "AutoComplete.h"
#include "model/core/SettingFieldVariant.h"
#include "model/basic/ResolveIndirection.h"
#include "MutableRxPipelineSettings.h"
#include "MutableTxPipelineSettings.h"
#include "RadioCategories.h"

class MutableBandSettings :
  public BandSettings<MutableRxPipelineSettings, MutableTxPipelineSettings>,
  public AutoComplete
{
public:
  MutableBandSettings(RadioSettings_BandSettingsPb& raw)
    : BandSettings(raw)
    , m_categories(nullptr)
  {}

  void setCategories(RadioCategories* categories)
  {
    m_categories = categories;
    m_pipelineA.base().setCategories(categories);
    m_pipelineB.base().setCategories(categories);
    m_txPipeline.base().setCategories(categories);
  }

  ResultCode setBandRequest(NameString& bandName);

  ResultCode applyBandDefaults(const ModeList& modeInfo);

  ResultCode autoComplete(
    const SettingFieldPath& path,
    uint32_t startingAtIndex,
    AutoCompleteTrigger trigger) override;

  ResultCode autoComplete() override;

  ResultCode autoCompleteBandRequest();
  ResultCode autoCompleteMultiPipeline();

protected:
  void copyBasicsForTxTracking(RadioSettings_RxPipelineSettingsPb& rxPipeline);

  RadioCategories* m_categories;
};
