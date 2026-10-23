#pragma once

#include <settings/model/radio/WithBandT.h>
#include <settings/model/radio/BandRfSettings.h>
#include <settings/model/radio/IfSettings.h>
#include <settings/model/proto/RadioSettings.pb.h>
#include <settings/model/radio/iq/RxPipelineSettings.h>
#include <settings/model/radio/IApplyBandDefaults.h>
#include <settings/model/radio/BandSettingsDefaults.h>

#include "SingleIqBandSettingsCache.h"


class SingleIqBandSettings
  : public IApplyBandDefaults
  , public BandSettingsDefaults
  , public WithBandT<
    makesdr_SingleIqBandSettingsPb,
    &makesdr_SingleIqBandSettingsPb_msg,
    makesdr_SingleIqBandSettingsPb_band_request_tag,
    makesdr_SingleIqBandSettingsPb_band_tag,
    SingleIqBandSettingsCache
  >
{
public:

  using Proto = makesdr_SingleIqBandSettingsPb;

  SingleIqBandSettings(Proto& rawSettings);

  [[nodiscard]] const Band * getBand() const { return &m_band; }
  [[nodiscard]] const Mode* getFocusMode() const { return &m_focusPipeline.base().mode(); }

  [[nodiscard]] bool hasRfSettings() const { return m_rawSettings.has_rf; }
  BandRfSettings* rfSettings() { return &m_rfSettings; }
  [[nodiscard]] const BandRfSettings* rfSettings() const { return &m_rfSettings; }

  [[nodiscard]] bool hasIfSettings() const { return m_rawSettings.has_if_; }
  [[nodiscard]] IfSettings* ifSettings() { return &m_ifSettings; }
  [[nodiscard]] const IfSettings* ifSettings() const { return &m_ifSettings; }

  [[nodiscard]] bool hasFocusPipeline() const { return m_rawSettings.has_focus_pipeline; }
  RxPipelineSettings* focusPipeline() { return &m_focusPipeline; }
  [[nodiscard]] const RxPipelineSettings* focusPipeline() const { return &m_focusPipeline; }

  ResultCode updateIndirectField(const FieldUpdate &settingUpdate, uint32_t startingAtIndex)
  {
    return ResultCode::ERR_SETTING_INDIRECT_PATH_INVALID; // No indirect fields in "Basic" classes
  }

  ResultCode autoComplete(const BandCategoryList* bands, const ModeList* modes, SingleIqBandSettingsCache* cache);
  ResultCode autoComplete(
    const FieldUpdate& setting,
    uint32_t startIndex,
    const BandCategoryList* bands,
    const ModeList* modes,
    SingleIqBandSettingsCache* cache
    );

  ResultCode applyBandDefaults(const Band& band, const BandCategoryList* bands, const ModeList* modes) override;

protected:
  Proto& m_rawSettings;
  BandRfSettings m_rfSettings;
  IfSettings m_ifSettings;
  RxPipelineSettings m_focusPipeline;
};
