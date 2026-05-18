#pragma once
#include "Band.h"
#include "../pipeline/PipelineId.h"
#include "../pipeline/RxPipelineSettings.h"
#include "../pipeline/TxPipelineSettings.h"
#include "../base/SettingsBase.h"
#include <etl/variant.h>

#include "etl/vector.h"

using BandOrRequestVariant = etl::variant<etl::monostate, StringRef, Band>;

class BandSettings : public SettingsBase
{
public:
  BandSettings(RadioSettings_BandSettingsPb& raw)
    : m_rawSettings(raw)
    , m_pipelineA(raw.pipeline_a)
    , m_pipelineB(raw.pipeline_b)
    , m_txPipeline(raw.tx_pipeline)
  {
    if (m_rawSettings.which_band_or_request == RadioSettings_BandSettingsPb_band_request_tag) {
      m_bandOrRequest.emplace<StringRef>(
        m_rawSettings.band_or_request.band_request,
        m_rawSettings.band_or_request.band_request,
        sizeof(m_rawSettings.band_or_request.band_request)
      );
    } else if (m_rawSettings.which_band_or_request == RadioSettings_BandSettingsPb_band_tag) {
      m_bandOrRequest.emplace<Band>(m_rawSettings.band_or_request.band);
    }
  }

  [[nodiscard]] bool isBandValid() const { return m_bandOrRequest.index() != 0; }
  [[nodiscard]] bool hasBandRequest() const { return m_bandOrRequest.index() == 1; }
  [[nodiscard]] bool hasBand() const { return m_bandOrRequest.index() == 2; }
  [[nodiscard]] bool hasFocusPipelineId() const { return m_rawSettings.has_focus_pipeline_id; }
  [[nodiscard]] bool hasTxPipelineId() const { return m_rawSettings.has_tx_pipeline_id; }
  [[nodiscard]] bool hasIsMultiPipeline() const { return m_rawSettings.has_is_multi_pipeline; }
  [[nodiscard]] bool hasPipelineA() const { return m_rawSettings.has_pipeline_a; }
  [[nodiscard]] bool hasPipelineB() const { return m_rawSettings.has_pipeline_b; }
  [[nodiscard]] bool hasTxPipeline() const { return m_rawSettings.has_tx_pipeline; }

  [[nodiscard]] const Band* band() const { return etl::get_if<Band>(&m_bandOrRequest); }

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

  RadioSettings_BandSettingsPb& raw() { return m_rawSettings; }

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
  BandOrRequestVariant m_bandOrRequest;
  RxPipelineSettings m_pipelineA;
  RxPipelineSettings m_pipelineB;
  TxPipelineSettings m_txPipeline;
  // BandVector m_bands;
};


