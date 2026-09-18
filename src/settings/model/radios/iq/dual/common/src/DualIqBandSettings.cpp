#include "settings/model/radios/iq/DualIqBandSettings.h"


  DualIqBandSettings::DualIqBandSettings(Proto& rawSettings)
    : WithBandT(rawSettings)
    , m_rawSettings(rawSettings)
    , m_rfSettings(rawSettings.rf)
    , m_ifSettings(rawSettings.if_)
    , m_pipeline_a(rawSettings.pipeline_a)
    , m_pipeline_b(rawSettings.pipeline_b)
  {
  }

const Mode*
DualIqBandSettings::getFocusMode() const
{
    const RxPipelineSettings* pipeline = focusPipeline();
    if (pipeline == nullptr) return nullptr;
    return &pipeline->base().mode();
}

bool
DualIqBandSettings::hasFocusPipeline() const
{
  if (m_rawSettings.has_focus_pipeline_id) {
    if (m_rawSettings.focus_pipeline_id == static_cast<int32_t>(PipelineId::A)) {
      return m_rawSettings.has_pipeline_a;
    } else {
      return m_rawSettings.has_pipeline_b;
    }
  }
  return false;
}

RxPipelineSettings*
DualIqBandSettings::focusPipeline()
{
  if (m_rawSettings.has_focus_pipeline_id) {
    if (m_rawSettings.focus_pipeline_id == static_cast<int32_t>(PipelineId::A)) {
      return &m_pipeline_a;
    } else {
      return &m_pipeline_b;
    }
  }
  return nullptr;
}

bool
DualIqBandSettings::hasPipeline(PipelineId pipelineId) const
{
  switch (pipelineId) {
    case PipelineId::A: return m_rawSettings.has_pipeline_a;
    case PipelineId::B: return m_rawSettings.has_pipeline_b;
    default: return false;
  }
}

RxPipelineSettings*
DualIqBandSettings::pipeline(PipelineId pipelineId)
{
  switch (pipelineId) {
    case PipelineId::A: return &m_pipeline_a;
    case PipelineId::B: return &m_pipeline_b;
    default: return nullptr;
  }
}

ResultCode
DualIqBandSettings::updateIndirectField(const FieldUpdate &settingUpdate, uint32_t startingAtIndex)
{
    const FieldPath& path = settingUpdate.descriptor().getPath();
    if (startingAtIndex >= path.size()) {
      return ResultCode::ERR_SETTING_INDIRECT_PATH_INVALID;
    }

    if (path[startingAtIndex] == makesdr_DualIqBandSettingsPb_focus_pipeline_tag) {
      auto pipelineId = static_cast<PipelineId>(m_rawSettings.focus_pipeline_id);
      if (pipelineId == PipelineId::NONE) {
        return ResultCode::ERR_SETTING_BAND_SETTINGS_FOCUS_PIPELINE_NOT_SET;
      }
      return MessageTraverser::updateField(
        pipelineId == PipelineId::A ? &m_pipeline_a : &m_pipeline_b,
        &makesdr_RxPipelineSettingsPb_msg,
        settingUpdate,
        startingAtIndex + 1
        );
    }
    return ResultCode::ERR_SETTING_INDIRECT_PATH_INVALID;
}

ResultCode
DualIqBandSettings::autoComplete(const BandCategoryList* bands, const ModeList* modes, DualIqBandSettingsCache* cache)
{
    ResultCode rcBand = autoCompleteBand(bands, modes, cache, this);
    ResultCode rcPipelineA = m_pipeline_a.autoComplete(modes);
    ResultCode rcPipelineB = m_pipeline_b.autoComplete(modes);
    if (rcPipelineA != ResultCode::OK) {
      return rcPipelineA;
    }
    if (rcPipelineB != ResultCode::OK) {
      return rcPipelineB;
    }
    if (rcBand != ResultCode::OK) {
      return rcBand;
    }
    return ResultCode::OK;

}

ResultCode
DualIqBandSettings::autoComplete(
  const FieldDescriptor& setting,
  uint32_t startIndex,
  const BandCategoryList* bands,
  const ModeList* modes,
  DualIqBandSettingsCache* cache
  )
{
    const FieldPath& path = setting.getPath();
    if (startIndex >= path.size()) {
      return ResultCode::ERR_SETTING_AUTOCOMPLETE_PATH_INVALID;
    }
    switch (path[startIndex]) {
    case makesdr_DualIqBandSettingsPb_band_request_tag:
      return autoCompleteBand(setting, startIndex + 1, bands, modes, cache, this);
    case makesdr_DualIqBandSettingsPb_pipeline_a_tag:
      return m_pipeline_a.autoComplete(setting, startIndex + 1, modes);
    case makesdr_DualIqBandSettingsPb_pipeline_b_tag:
      return m_pipeline_b.autoComplete(setting, startIndex + 1, modes);
    case makesdr_DualIqBandSettingsPb_focus_pipeline_tag:
      {
        RxPipelineSettings* pipeline = focusPipeline();
        if (pipeline == nullptr) return ResultCode::ERR_SETTING_AUTOCOMPLETE_NO_FOCUS_PIPELINE;
        return pipeline->autoComplete(setting, startIndex + 1, modes);
      }
    case makesdr_DualIqBandSettingsPb_is_multi_pipeline_tag:
      return autoCompleteMultiPipeline();
    }

    return ResultCode::ERR_SETTING_AUTOCOMPLETE_NOT_IMPLEMENTED;
}

ResultCode
DualIqBandSettings::autoCompleteMultiPipeline()
{
  if (m_rawSettings.is_multi_pipeline) {
    if (!m_rawSettings.has_pipeline_b) {
      m_rawSettings.pipeline_b = m_rawSettings.pipeline_a;
      m_rawSettings.has_pipeline_b = true;
    }
    if (!m_rawSettings.has_focus_pipeline_id) {
      m_rawSettings.focus_pipeline_id = makesdr_PipelineId_PIPELINE_B;
      m_rawSettings.has_focus_pipeline_id = true;
    }
  } else {
    if (!m_rawSettings.has_focus_pipeline_id) {
      m_rawSettings.focus_pipeline_id = makesdr_PipelineId_PIPELINE_A;
      m_rawSettings.has_focus_pipeline_id = true;
    }
  }
  return ResultCode::OK;
}

ResultCode
DualIqBandSettings::applyBandDefaults(const Band& band, const BandCategoryList* bands, const ModeList* modes)
{
  ResultCode rc = m_rfSettings.applyBandDefaults(band, bands, modes);
  if (rc != ResultCode::OK) return rc;
  rc = m_pipeline_a.base().applyBandDefaults(band, bands, modes);
  if (rc != ResultCode::OK) return rc;
  return  m_pipeline_b.base().applyBandDefaults(band, bands, modes);
}
