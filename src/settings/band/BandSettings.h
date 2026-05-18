#pragma once
#include "Band.h"
#include "../pipeline/PipelineId.h"
#include "../pipeline/RxPipelineSettings.h"
#include "../pipeline/TxPipelineSettings.h"
#include "../base/SettingsBase.h"
#include <etl/variant.h>

#include "etl/vector.h"

// using BandIdVariant = etl::variant<etl::monostate, StringRef, uint32_t>;

class BandSettings : public SettingsBase
{
public:
  using BandVector = etl::vector<Band, 2>;

  BandSettings(RadioSettings_BandSettingsPb& raw)
    : m_rawSettings(raw)
    , m_band(raw.band)
    // , m_name{raw.name, raw.name, sizeof(raw.name)}
    , m_pipelineA(raw.pipeline_a)
    , m_pipelineB(raw.pipeline_b)
    , m_txPipeline(raw.tx_pipeline)
  {
    // if (m_rawSettings.which_band_id == RadioSettings_BandSettingsPb_band_name_tag) {
    //   m_bandId.emplace<StringRef>(m_rawSettings.band_id.band_name, m_rawSettings.band_id.band_name, sizeof(m_rawSettings.band_id.band_name));
    // } else if (m_rawSettings.which_band_id == RadioSettings_BandSettingsPb_band_index_tag) {
    //   m_bandId.emplace<uint32_t>(m_rawSettings.band_id.band_index);
    // }
    // for (int i = 0; i < m_rawSettings.bands_count; i++) {
    //   m_bands.emplace_back(m_rawSettings.bands[i]);
    // }
  }

  [[nodiscard]] bool hasBand() const { return m_rawSettings.has_band != 0; }
  [[nodiscard]] bool hasFocusPipelineId() const { return m_rawSettings.has_focus_pipeline_id; }
  [[nodiscard]] bool hasTxPipelineId() const { return m_rawSettings.has_tx_pipeline_id; }
  [[nodiscard]] bool hasIsMultiPipeline() const { return m_rawSettings.has_is_multi_pipeline; }
  [[nodiscard]] bool hasPipelineA() const { return m_rawSettings.has_pipeline_a; }
  [[nodiscard]] bool hasPipelineB() const { return m_rawSettings.has_pipeline_b; }
  [[nodiscard]] bool hasTxPipeline() const { return m_rawSettings.has_tx_pipeline; }

  Band& band() { return m_band; }

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

  // uint32_t bandsCount() const { return m_rawSettings.bands_count; }
  // BandVector& bands() { return m_bands; }
  //
  // Band* getBand()
  // {
  //   if (m_rawSettings.bands_count > 0) {
  //     if (m_rawSettings.which_band_id == RadioSettings_BandSettingsPb) {
  //       return getBand(m_rawSettings.band_id.band_index);
  //     }
  //     if (m_rawSettings.which_band_id == RadioSettings_BandSettingsPb) {
  //       return getBand(m_rawSettings.band_id.band_name);
  //     }
  //   }
  //   return nullptr;
  // }

protected:
  RadioSettings_BandSettingsPb& m_rawSettings;
  Band m_band;
  RxPipelineSettings m_pipelineA;
  RxPipelineSettings m_pipelineB;
  TxPipelineSettings m_txPipeline;
  // BandVector m_bands;
};


