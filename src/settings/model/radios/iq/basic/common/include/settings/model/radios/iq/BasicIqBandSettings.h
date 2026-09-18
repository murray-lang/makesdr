#pragma once

#include <settings/model/radios/basic/WithBandT.h>
#include <settings/model/radios/BandRfSettings.h>
#include <settings/model/radios/IfSettings.h>
#include <settings/model/proto/RadioSettings.pb.h>
#include "RxPipelineSettings.h"
#include <settings/model/radios/IBandSettings.h>

#include "BasicIqBandSettingsCache.h"


class BasicIqBandSettings
  : public IBandSettings
  , public WithBandT<
    makesdr_BasicIqBandSettingsPb,
    makesdr_BasicIqBandSettingsPb_band_request_tag,
    makesdr_BasicIqBandSettingsPb_band_tag,
    BasicIqBandSettingsCache
  >
{
public:

  using Proto = makesdr_BasicIqBandSettingsPb;

  BasicIqBandSettings(Proto& rawSettings);

  [[nodiscard]] const Band * getBand() const override { return &m_band; }
  [[nodiscard]] const Mode* getFocusMode() const override { return &m_focusPipeline.base().mode(); }

  [[nodiscard]] bool hasRfSettings() const override { return m_rawSettings.has_rf; }
  BandRfSettings* rfSettings() override { return &m_rfSettings; }
  [[nodiscard]] const BandRfSettings* rfSettings() const override { return &m_rfSettings; }

  [[nodiscard]] bool hasIfSettings() const override { return m_rawSettings.has_if_; }
  [[nodiscard]] IfSettings* ifSettings() override { return &m_ifSettings; }
  [[nodiscard]] const IfSettings* ifSettings() const override { return &m_ifSettings; }

  [[nodiscard]] bool hasFocusPipeline() const override { return m_rawSettings.has_focus_pipeline; }
  RxPipelineSettings* focusPipeline() override { return &m_focusPipeline; }
  [[nodiscard]] const RxPipelineSettings* focusPipeline() const override { return &m_focusPipeline; }

  ResultCode updateIndirectField(const FieldUpdate &settingUpdate, uint32_t startingAtIndex) override
  {
    return ResultCode::ERR_SETTING_INDIRECT_PATH_INVALID; // No indirect fields in "Basic" classes
  }

  ResultCode autoComplete(const BandCategoryList* bands, const ModeList* modes, BasicIqBandSettingsCache* cache);
  ResultCode autoComplete(
    const FieldDescriptor& setting,
    uint32_t startIndex,
    const BandCategoryList* bands,
    const ModeList* modes,
    BasicIqBandSettingsCache* cache
    );

  ResultCode applyBandDefaults(const Band& band, const BandCategoryList* bands, const ModeList* modes) override;

protected:
  Proto& m_rawSettings;
  BandRfSettings m_rfSettings;
  IfSettings m_ifSettings;
  RxPipelineSettings m_focusPipeline;
};
