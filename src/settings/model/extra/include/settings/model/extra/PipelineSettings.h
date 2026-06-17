#pragma once
#include <CrossPlatformTypes.h>

#include "AutoComplete.h"
#include "SettingsBase.h"
#include "RfSettings.h"
#include "Band.h"
#include "../../../../../meta/include/settings/model/meta/RadioMeta.h"


using ModeOrRequestVariant = variant<monostate, Mode::Type, Mode>;

class PipelineSettings : public SettingsBase, public AutoComplete
{
public:
  PipelineSettings(makesdr_PipelineSettingsPb& rawSettings);

  [[nodiscard]] bool isModeValid() const { return m_modeOrRequest.index() != 0; }
  [[nodiscard]] bool hasModeRequest() const { return m_modeOrRequest.index() == 1; }
  [[nodiscard]] bool hasMode() const { return m_modeOrRequest.index() == 2; }
  [[nodiscard]] bool hasRf() const { return m_rawSettings.has_rf; }

  Mode* mode() { return get_if<Mode>(&m_modeOrRequest); }
  Mode::Type* modeRequest() { return get_if<Mode::Type>(&m_modeOrRequest); }

  RfSettings& rfSettings() { return m_rfSettings; }
  [[nodiscard]] const RfSettings& rfSettings() const { return m_rfSettings; }

  void setCategories(RadioMeta* categories) { m_categories = categories; }

  ResultCode applyBandDefaults(const Band* pBand, const ModeList& modeInfo);

  ResultCode autoComplete(
    const SettingPath& path,
    uint32_t startingAtIndex,
    AutoCompleteTrigger trigger) override;

  ResultCode autoComplete() override;

  ResultCode autoCompleteMode();


protected:
  makesdr_PipelineSettingsPb& m_rawSettings;
  ModeOrRequestVariant m_modeOrRequest;
  RfSettings m_rfSettings;

  RadioMeta* m_categories;
};