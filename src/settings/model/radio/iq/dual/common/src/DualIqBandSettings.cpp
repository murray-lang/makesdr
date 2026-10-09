#include "settings/model/radio/iq/DualIqBandSettings.h"
#include <settings/model/update/MessageTraverser.h>

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
  switch (m_rawSettings.focus_pipeline_id)
  {
  case makesdr_PipelineId_PIPELINE_A: return &m_pipeline_a;
  case makesdr_PipelineId_PIPELINE_B: return &m_pipeline_b;
  default: return nullptr;
  }
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
      ResultCode rc = MessageTraverser::updateField(
        pipelineId == PipelineId::A ? &m_pipeline_a : &m_pipeline_b,
        &makesdr_RxPipelineSettingsPb_msg,
        settingUpdate,
        startingAtIndex + 1
        );
      if (rc == ResultCode::OK) {
        if (pipelineId == PipelineId::A) {
          m_rawSettings.has_pipeline_a = true;
        } else {
          m_rawSettings.has_pipeline_b = true;
        }
      }
      return rc;
    }
    return MessageTraverser::updateField(
      &m_rawSettings,
      &makesdr_DualIqBandSettingsPb_msg,
      settingUpdate,
      startingAtIndex
      );
}

ResultCode
DualIqBandSettings::autoComplete(const BandCategoryList* bands, const ModeList* modes, DualIqBandSettingsCache* cache)
{
    ResultCode rcBand = autoCompleteBand(nullptr, bands, modes, cache, this);
    ResultCode rcPipelineA = m_pipeline_a.autoComplete(modes);
    if (rcPipelineA == ResultCode::OK) {
      m_rawSettings.has_pipeline_a = true;
    }
    ResultCode rcPipelineB = m_pipeline_b.autoComplete(modes);
    if (rcPipelineB != ResultCode::OK) {
      m_rawSettings.has_pipeline_b = true;
    }
    if (rcPipelineA!= ResultCode::OK) {
      return rcPipelineA;
    }
    if (rcPipelineB!= ResultCode::OK) {
      return rcPipelineB;
    }
    if (rcBand != ResultCode::OK) {
      return rcBand;
    }
    return ResultCode::OK;

}

ResultCode
DualIqBandSettings::autoComplete(
  const FieldUpdate& setting,
  uint32_t startIndex,
  const BandCategoryList* bands,
  const ModeList* modes,
  DualIqBandSettingsCache* cache
  )
{
    const FieldPath& path = setting.path();
    if (startIndex >= path.size()) {
      return ResultCode::ERR_SETTING_AUTOCOMPLETE_PATH_INVALID;
    }
    ResultCode rc = ResultCode::ERR_SETTING_AUTOCOMPLETE_NOT_IMPLEMENTED;
    switch (path[startIndex]) {
    case makesdr_DualIqBandSettingsPb_band_request_tag:
      return autoCompleteBand(setting, startIndex + 1, bands, modes, cache, this);

    case makesdr_DualIqBandSettingsPb_pipeline_a_tag:
      rc = m_pipeline_a.autoComplete(setting, startIndex + 1, modes);
      if (rc == ResultCode::OK) {
        m_rawSettings.has_pipeline_a = true;
      }
      break;
    case makesdr_DualIqBandSettingsPb_pipeline_b_tag:
      rc = m_pipeline_b.autoComplete(setting, startIndex + 1, modes);
      if (rc == ResultCode::OK) {
        m_rawSettings.has_pipeline_b = true;
      }
      break;
    case makesdr_DualIqBandSettingsPb_focus_pipeline_tag:
      {
        RxPipelineSettings* pipeline = focusPipeline();
        if (pipeline == nullptr) return ResultCode::ERR_SETTING_AUTOCOMPLETE_NO_FOCUS_PIPELINE;
        rc = pipeline->autoComplete(setting, startIndex + 1, modes);
        if (rc == ResultCode::OK) {
          if (focusPipelineId() == PipelineId::A) {
            m_rawSettings.has_pipeline_a = true;
          } else {
            m_rawSettings.has_pipeline_b = true;
          }
        }
      }
      break;
    case makesdr_DualIqBandSettingsPb_is_multi_pipeline_tag:
      rc = autoCompleteMultiPipeline();
      if (rc == ResultCode::OK) {
        m_rawSettings.has_is_multi_pipeline = true;
      }
      break;
    }

    return rc;
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
    m_rawSettings.has_rf = true;
  rc = m_pipeline_a.base().applyBandDefaults(band, bands, modes);
  if (rc != ResultCode::OK) return rc;
  m_rawSettings.has_pipeline_a = true;
  m_rawSettings.pipeline_a.has_base = true;
  rc = m_pipeline_b.base().applyBandDefaults(band, bands, modes);
  if (rc != ResultCode::OK) return rc;
  m_rawSettings.has_pipeline_b = true;
  m_rawSettings.pipeline_b.has_base = true;
  return rc;
}
