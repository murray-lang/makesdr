#pragma once

#include <settings/model/radios/PipelineId.h>
#include <settings/model/radios/basic/WithBandT.h>
#include <settings/model/radios/BandRfSettings.h>
#include <settings/model/radios/IfSettings.h>
#include <settings/model/proto/RadioSettings.pb.h>
#include <settings/model/radios/iq/RxPipelineSettings.h>

#include <settings/model/radios/IBandSettings.h>

#include "DualIqBandSettingsCache.h"


class DualIqBandSettings
  : public WithBandT<
    makesdr_DualIqBandSettingsPb,
    makesdr_DualIqBandSettingsPb_band_request_tag,
    makesdr_DualIqBandSettingsPb_band_tag,
    DualIqBandSettingsCache
  >
  , public IBandSettings
{
public:

  using Proto = makesdr_DualIqBandSettingsPb;

  DualIqBandSettings(Proto& rawSettings);

  [[nodiscard]] const Band * getBand() const override { return &m_band; }
  [[nodiscard]] const Mode* getFocusMode() const override;
  [[nodiscard]] bool hasRfSettings() const override { return m_rawSettings.has_rf; }
  BandRfSettings* rfSettings() override { return &m_rfSettings; }
  [[nodiscard]] const BandRfSettings* rfSettings() const override { return &m_rfSettings; }

  [[nodiscard]] bool hasIfSettings() const override { return m_rawSettings.has_if_; }
  [[nodiscard]] IfSettings* ifSettings() override { return &m_ifSettings; }
  [[nodiscard]] const IfSettings* ifSettings() const override { return &m_ifSettings; }

  [[nodiscard]] bool hasFocusPipelineId() const override { return m_rawSettings.has_focus_pipeline_id; }
  [[nodiscard]] PipelineId focusPipelineId() const override
  {
    return static_cast<PipelineId>(m_rawSettings.focus_pipeline_id);
  }
  [[nodiscard]] bool hasFocusPipeline() const override;

  RxPipelineSettings* focusPipeline() override;
  [[nodiscard]] const RxPipelineSettings* focusPipeline() const override
  {
    return const_cast<DualIqBandSettings*>(this)->focusPipeline();
  }

  [[nodiscard]] bool hasPipeline(PipelineId pipelineId) const override;
  RxPipelineSettings* pipeline(PipelineId pipelineId) override;
  [[nodiscard]] const RxPipelineSettings* pipeline(PipelineId pipelineId) const override
  {
    return const_cast<DualIqBandSettings*>(this)->pipeline(pipelineId);
  }

  [[nodiscard]] bool hasIsMultiPipeline() const override { return m_rawSettings.has_is_multi_pipeline; }
  [[nodiscard]] bool isMultiPipeline() const override
  {
    return m_rawSettings.has_is_multi_pipeline ? m_rawSettings.is_multi_pipeline : false;
  }

  ResultCode updateIndirectField(const FieldUpdate &settingUpdate, uint32_t startingAtIndex) override;

  ResultCode autoComplete(const BandCategoryList* bands, const ModeList* modes, DualIqBandSettingsCache* cache);
  ResultCode autoComplete(
    const FieldDescriptor& setting,
    uint32_t startIndex,
    const BandCategoryList* bands,
    const ModeList* modes,
    DualIqBandSettingsCache* cache
    );

  ResultCode autoCompleteMultiPipeline();

  ResultCode applyBandDefaults(const Band& band, const BandCategoryList* bands, const ModeList* modes) override;

  // [[nodiscard]] bool hasPipelineA() const { return m_rawSettings.has_pipeline_a; }
  // RxPipelineSettings& pipelineA() { return m_pipeline_a; }
  // [[nodiscard]] const RxPipelineSettings& pipelineA() const { return m_pipeline_a; }
  //
  // [[nodiscard]] bool hasPipelineB() const { return m_rawSettings.has_pipeline_b; }
  // RxPipelineSettings& pipelineB() { return m_pipeline_b; }
  // [[nodiscard]] const RxPipelineSettings& pipelineB() const { return m_pipeline_b; }



protected:
  Proto& m_rawSettings;
  BandRfSettings m_rfSettings;
  IfSettings m_ifSettings;
  RxPipelineSettings m_pipeline_a;
  RxPipelineSettings m_pipeline_b;
};
