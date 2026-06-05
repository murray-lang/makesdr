#pragma once
#include <CrossPlatformTypes.h>
#include "Band.h"
#include "../../../../../core/include/settings/model/core/PipelineId.h"
#include "settings/model/core/SettingsBase.h"
#include "ResolveIndirection.h"
#include "RxPipelineSettings.h"
#include "TxPipelineSettings.h"

using BandOrRequestVariant = variant<monostate, StringRef, Band>;

class BandSettings : public SettingsBase, public ResolveIndirection, public AutoComplete
{
public:
  BandSettings(RadioSettings_BandSettingsPb& raw)
  : m_rawSettings(raw)
    , m_pipelineA(raw.pipeline_a)
    , m_pipelineB(raw.pipeline_b)
    , m_txPipeline(raw.tx_pipeline)
  {
    setBandOrRequestVariant(m_rawSettings);
  }

  BandSettings& operator=(const BandSettings& rhs) noexcept
  {
    if (this != &rhs) {
      operator=(rhs.m_rawSettings);
    }
    return *this;
  }

  BandSettings& operator=(const RadioSettings_BandSettingsPb& rhs) noexcept
  {
    m_rawSettings = rhs;
    setBandOrRequestVariant(m_rawSettings);
    return *this;
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

  [[nodiscard]] const StringRef* bandName() const
  {
    const StringRef* request = get_if<StringRef>(&m_bandOrRequest);
    if (request != nullptr) return request;
    const Band* band = get_if<Band>(&m_bandOrRequest);
    if (band != nullptr) return &band->name();
    return nullptr;
  }

  [[nodiscard]] const Band* band() const { return get_if<Band>(&m_bandOrRequest); }

  RxPipelineSettings& pipelineASettings() { return m_pipelineA; }
  [[nodiscard]] const RxPipelineSettings& pipelineASettings() const { return m_pipelineA; }

  RxPipelineSettings& pipelineBSettings() { return m_pipelineB; }
  [[nodiscard]] const RxPipelineSettings& pipelineBSettings() const { return m_pipelineB; }

  TxPipelineSettings& txPipelineSettings() { return m_txPipeline; }
  [[nodiscard]] const TxPipelineSettings& txPipelineSettings() const { return m_txPipeline; }

  [[nodiscard]] bool isMultiPipeline() const { return m_rawSettings.is_multi_pipeline; }
  [[nodiscard]] PipelineId focusPipelineId() const { return static_cast<PipelineId>(m_rawSettings.focus_pipeline_id); }
  [[nodiscard]] PipelineId txPipelineId() const { return static_cast<PipelineId>(m_rawSettings.tx_pipeline_id); }

  [[nodiscard]] const RxPipelineSettings* getPipelineSettings(PipelineId pipelineId) const
  {
    if (pipelineId == PipelineId::A) return &m_pipelineA;
    if (pipelineId == PipelineId::B) return &m_pipelineB;
    return nullptr;
  }

  [[nodiscard]] const RxPipelineSettings* getFocusPipelineSettings() const
  {
    if (!hasFocusPipelineId()) return nullptr;
    return getPipelineSettings(focusPipelineId());
  }

  RadioSettings_BandSettingsPb& raw() { return m_rawSettings; }
  [[nodiscard]] const RadioSettings_BandSettingsPb& raw() const { return m_rawSettings; }

  ResultCode resolveIndirection(
   const SettingFieldPath& indirectPath,
   uint32_t startingAtIndex,
   SettingFieldPath& resolvedPath
 ) override
  {
    if (startingAtIndex >= indirectPath.size()) {
      return ResultCode::ERR_SETTING_RESOLVE_INDIRECTION_PATH_INVALID;
    }
    uint32_t nextIndex = startingAtIndex;
    switch (indirectPath[startingAtIndex]) {
    case RadioSettings_BandSettingsPb_focus_pipeline_tag:
      {
        PipelineId pipelineId = focusPipelineId();
        if (pipelineId == PipelineId::A) {
          resolvedPath.emplace_back(RadioSettings_BandSettingsPb_pipeline_a_tag);
        } else if (pipelineId == PipelineId::B) {
          resolvedPath.emplace_back(RadioSettings_BandSettingsPb_pipeline_b_tag);
        } else {
          return ResultCode::ERR_SETTING_BAND_SETTINGS_FOCUS_PIPELINE_NOT_SET;
        }
        nextIndex++;
      }
      break;
    default: break;
    }
    // We happen to know that pipelines have no indirect paths (for now). Just copy the rest.
    for (uint32_t i = nextIndex; i < indirectPath.size(); i++) {
      resolvedPath.emplace_back(indirectPath[i]);
    }
    return ResultCode::OK;
  }

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

  void setBandOrRequestVariant(RadioSettings_BandSettingsPb& raw)
  {
    if (raw.which_band_or_request == RadioSettings_BandSettingsPb_band_request_tag) {
      m_bandOrRequest.emplace<StringRef>(
        raw.band_or_request.band_request,
        raw.band_or_request.band_request,
        sizeof(raw.band_or_request.band_request)
      );
    } else if (raw.which_band_or_request == RadioSettings_BandSettingsPb_band_tag) {
      m_bandOrRequest.emplace<Band>(raw.band_or_request.band);
    }
  }
  void setBandOrRequestVariant() { setBandOrRequestVariant(m_rawSettings); }

  RadioSettings_BandSettingsPb& m_rawSettings;
  BandOrRequestVariant m_bandOrRequest;
  RxPipelineSettings m_pipelineA;
  RxPipelineSettings m_pipelineB;
  TxPipelineSettings m_txPipeline;
  // BandVector m_bands;

  RadioCategories* m_categories;
};
