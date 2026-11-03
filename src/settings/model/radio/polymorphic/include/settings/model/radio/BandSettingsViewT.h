#pragma once
#include "IBandSettingsView.h"

// Adapts a concrete band settings class to IBandSettingsView.
// Header-only and only instantiated by the front end, via RadioSettingsViewT.
template<typename BandSettingsT>
class BandSettingsViewT final : public IBandSettingsView
{
public:
  // pBand may be nullptr when the radio doesn't have the band: the view is then not handed out.
  explicit BandSettingsViewT(const BandSettingsT* pBand) : m_pBand(pBand) {}
  BandSettingsViewT(const BandSettingsViewT&) = delete;
  BandSettingsViewT& operator=(const BandSettingsViewT&) = delete;

  [[nodiscard]] bool isValid() const { return m_pBand != nullptr; }

  [[nodiscard]] const Band* getBand() const override { return m_pBand->getBand(); }
  [[nodiscard]] const Mode* getFocusMode() const override { return m_pBand->getFocusMode(); }

  [[nodiscard]] bool hasRfSettings() const override { return m_pBand->hasRfSettings(); }
  [[nodiscard]] const BandRfSettings* rfSettings() const override { return m_pBand->rfSettings(); }

  [[nodiscard]] bool hasFocusPipelineId() const override { return m_pBand->hasFocusPipelineId(); }

  // A single-pipeline band's only pipeline is A.
  [[nodiscard]] PipelineId focusPipelineId() const override
  {
    const PipelineId pipelineId = m_pBand->focusPipelineId();
    return pipelineId == PipelineId::NONE && m_pBand->focusPipeline() != nullptr ? PipelineId::A : pipelineId;
  }

  [[nodiscard]] bool hasFocusPipeline() const override { return m_pBand->hasFocusPipeline(); }
  [[nodiscard]] const RxPipelineSettings* focusPipeline() const override { return m_pBand->focusPipeline(); }

  [[nodiscard]] const RxPipelineSettings* pipeline(PipelineId pipelineId) const override
  {
    const RxPipelineSettings* pipeline = m_pBand->pipeline(pipelineId);
    if (pipeline == nullptr && pipelineId == PipelineId::A) {
      return m_pBand->focusPipeline();
    }
    return pipeline;
  }

  [[nodiscard]] bool hasIsMultiPipeline() const override { return m_pBand->hasIsMultiPipeline(); }
  [[nodiscard]] bool isMultiPipeline() const override { return m_pBand->isMultiPipeline(); }

  [[nodiscard]] bool hasTxPipeline() const override
  {
    if constexpr (requires { m_pBand->hasTxPipeline(); }) {
      return m_pBand->hasTxPipeline();
    } else {
      return false;
    }
  }

  [[nodiscard]] bool hasTxPipelineId() const override
  {
    if constexpr (requires { m_pBand->hasTxPipelineId(); }) {
      return m_pBand->hasTxPipelineId();
    } else {
      return false;
    }
  }

  [[nodiscard]] PipelineId txPipelineId() const override
  {
    if constexpr (requires { m_pBand->txPipelineId(); }) {
      return m_pBand->txPipelineId();
    } else {
      return PipelineId::NONE;
    }
  }

private:
  const BandSettingsT* m_pBand;
};
