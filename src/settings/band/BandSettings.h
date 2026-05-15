#pragma once
#include "Band.h"
#include "../pipeline/PipelineId.h"
#include "../pipeline/RxPipelineSettings.h"
#include "../pipeline/TxPipelineSettings.h"
#include "../base/SettingsBase.h"


class BandSettings : public SettingsBase
{
public:
  BandSettings(RadioSettings_BandSettingsPb& raw)
    : SettingsBase(&RadioSettings_BandSettingsPb_msg)
    , m_rawSettings(raw)
    , m_name{raw.name, raw.name, sizeof(raw.name)}
    , m_pipelineA(raw.pipeline_a)
    , m_pipelineB(raw.pipeline_b)
    , m_txPipeline(raw.tx_pipeline)
  {}

  [[nodiscard]] bool hasName() const { return m_rawSettings.has_name; }
  [[nodiscard]] bool hasFocusPipelineId() const { return m_rawSettings.has_focus_pipeline_id; }
  [[nodiscard]] bool hasTxPipelineId() const { return m_rawSettings.has_tx_pipeline_id; }
  [[nodiscard]] bool hasIsMultiPipeline() const { return m_rawSettings.has_is_multi_pipeline; }
  [[nodiscard]] bool hasPipelineA() const { return m_rawSettings.has_pipeline_a; }
  [[nodiscard]] bool hasPipelineB() const { return m_rawSettings.has_pipeline_b; }
  [[nodiscard]] bool hasTxPipeline() const { return m_rawSettings.has_tx_pipeline; }

  StringRef& name() { return m_name; }

  RxPipelineSettings& pipelineASettings() { return m_pipelineA; }
  [[nodiscard]] const RxPipelineSettings& pipelineASettings() const { return m_pipelineA; }

  RxPipelineSettings& pipelineBSettings() { return m_pipelineB; }
  [[nodiscard]] const RxPipelineSettings& pipelineBSettings() const { return m_pipelineB; }

  TxPipelineSettings& txPipelineSettings() { return m_txPipeline; }
  [[nodiscard]] const TxPipelineSettings& txPipelineSettings() const { return m_txPipeline; }

  [[nodiscard]] bool isMultiPipeline() const { return m_rawSettings.is_multi_pipeline; }
  [[nodiscard]] PipelineId focusPipelineId() const { return static_cast<PipelineId>(m_rawSettings.focus_pipeline_id); }
  [[nodiscard]] PipelineId txPipelineId() const { return static_cast<PipelineId>(m_rawSettings.tx_pipeline_id); }

  RxPipelineSettings* getPipelineSettings(PipelineId pipelineId)
  {
    if (pipelineId == PipelineId::A) return &m_pipelineA;
    if (pipelineId == PipelineId::B) return &m_pipelineB;
    return nullptr;
  }

protected:
  void* getMessage() override { return &m_rawSettings; }
  StringRef m_name;
  RadioSettings_BandSettingsPb& m_rawSettings;
  RxPipelineSettings m_pipelineA;
  RxPipelineSettings m_pipelineB;
  TxPipelineSettings m_txPipeline;
};


