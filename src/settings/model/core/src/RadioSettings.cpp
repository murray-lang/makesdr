#include "settings/model/core/RadioSettings.h"

RadioSettings::RadioSettings(RadioSettings_RadioSettingsPb& raw)
    : m_rawSettings(raw)
    , m_visitor(&raw, &RadioSettings_RadioSettingsPb_msg)
  {
    InitBandAndPipelineIdsWithDefaults();
  }

ResultCode
RadioSettings::updateField(const SettingFieldUpdate &settingUpdate)
{
  ResultCode rc = ResultCode::OK;
  if (settingUpdate.isIndirect()) {
    SettingFieldPath resolvedPath;
    rc = resolveIndirection(settingUpdate.path(), 0, resolvedPath);
    if (rc == ResultCode::OK) {
      rc = m_visitor.updateField(resolvedPath, settingUpdate.value());
    }
  } else {
    rc = m_visitor.updateField(settingUpdate);
  }
  return rc;
}

ResultCode
RadioSettings::updateField(const SettingFieldPath &path, const SettingFieldVariant &value)
{
  return m_visitor.updateField(path, value);
}

ResultCode
RadioSettings::getField(const SettingFieldPath &path, SettingFieldVariant &value) const
{
  return m_visitor.getField(path, value);
}

ResultCode
RadioSettings::getField(
  const SettingFieldPath &path,
  SettingFieldVariant &value,
  bool mustHave,
  bool parentsMustHave,
  bool& retrieved
)
{
  return m_visitor.getField(path, value, mustHave, parentsMustHave, retrieved);
}

ResultCode
RadioSettings::setFieldPresence(const SettingFieldPath &path, bool present)
{
  return m_visitor.setFieldPresence(path, present);
}

ResultCode
RadioSettings::mergePresentFields(const void* pRhsMessage)
{
  return m_visitor.mergePresentFields(pRhsMessage);
}

ResultCode
RadioSettings::resolveDottedPath(
  const char *dottedPath,
  SettingFieldPath &path,
  bool* isIndirectOut,
  AutoCompleteTrigger* triggerOut
)
{
  return MessageVisitor::resolveDottedPath(dottedPath, path, isIndirectOut, triggerOut);
}

[[nodiscard]] const RadioSettings_BandSettingsPb*
RadioSettings::getBandSettings(SplitBandId bandId) const
{
  switch (bandId)
  {
  case SplitBandId::One: return &m_rawSettings.active_bands.band_1;
  case SplitBandId::Two: return &m_rawSettings.active_bands.band_2;
  default: return nullptr;
  }
}

// [[nodiscard]] uint32_t getFocusBandTag() const
// {
//   switch (m_rawSettings.active_bands.focus_band_id)
//   {
//   case RadioSettings_SplitBandId_SPLIT_BAND_ONE: return RadioSettings_ActiveBandSettingsPb_band_1_tag;
//   case RadioSettings_SplitBandId_SPLIT_BAND_TWO: return RadioSettings_ActiveBandSettingsPb_band_2_tag;
//   default: return 0;
//   }
// }
//
// [[nodiscard]] uint32_t getFocusPipelineTag(uint32_t bandTag) const
// {
//   switch(bandTag) {
//     case RadioSettings_ActiveBandSettingsPb_band_1_tag:
//     return getPipelineTag(static_cast<PipelineId>(m_rawSettings.active_bands.band_1.focus_pipeline_id));
//     case RadioSettings_ActiveBandSettingsPb_band_2_tag:
//     return getPipelineTag(static_cast<PipelineId>(m_rawSettings.active_bands.band_2.focus_pipeline_id));
//     default: return 0;
//   }
// }
//
// [[nodiscard]] uint32_t getPipelineTag(PipelineId pipelineId) const
// {
//   switch (pipelineId) {
//   case PipelineId::A: return RadioSettings_BandSettingsPb_pipeline_a_tag;
//   case PipelineId::B: return RadioSettings_BandSettingsPb_pipeline_b_tag;
//   default: return 0;
//   }
// }

void
RadioSettings::ptt(bool on)
{
  m_rawSettings.ptt = on;
  m_rawSettings.has_ptt = true;
}

[[nodiscard]] PipelineId
RadioSettings::getFocusPipelineId(SplitBandId bandId) const
{
  switch (bandId) {
  case SplitBandId::One: return static_cast<PipelineId>(m_rawSettings.active_bands.band_1.focus_pipeline_id);
  case SplitBandId::Two: return static_cast<PipelineId>(m_rawSettings.active_bands.band_2.focus_pipeline_id);
  default: return PipelineId::NONE;
  }
}

[[nodiscard]] PipelineId
RadioSettings::getFocusBandFocusPipelineId() const
{
  switch (m_rawSettings.active_bands.focus_band_id) {
  case RadioSettings_SplitBandId_SPLIT_BAND_ONE:
    return static_cast<PipelineId>(m_rawSettings.active_bands.band_1.focus_pipeline_id);
  case RadioSettings_SplitBandId_SPLIT_BAND_TWO:
    return static_cast<PipelineId>(m_rawSettings.active_bands.band_2.focus_pipeline_id);
  default: return PipelineId::NONE;
  }
}

[[nodiscard]] const RadioSettings_BandSettingsPb*
RadioSettings::getFocusBandSettings() const
{
  return getBandSettings(static_cast<SplitBandId>(m_rawSettings.active_bands.focus_band_id));
}

[[nodiscard]] const RadioSettings_BandSettingsPb*
RadioSettings::getTxBandSettings() const
{
  return getBandSettings(static_cast<SplitBandId>(m_rawSettings.active_bands.tx_band_id));
}

[[nodiscard]] const RadioSettings_RxPipelineSettingsPb*
  RadioSettings::getFocusRxPipelineSettings(SplitBandId bandId) const
{
  const RadioSettings_BandSettingsPb* pBandSettings = getBandSettings(bandId);
  if (pBandSettings == nullptr) return nullptr;
  switch (pBandSettings->focus_pipeline_id)
  {
    case RadioSettings_PipelineId_PIPELINE_A: return &pBandSettings->pipeline_a;
    case RadioSettings_PipelineId_PIPELINE_B: return &pBandSettings->pipeline_b;
    default: return nullptr;
  }
}

[[nodiscard]] const RadioSettings_RxPipelineSettingsPb*
RadioSettings::getTxPipelineSettings() const
{
  const RadioSettings_BandSettingsPb* pBandSettings = getTxBandSettings();
  if (pBandSettings == nullptr) return nullptr;
  switch (pBandSettings->tx_pipeline_id)
  {
  case RadioSettings_PipelineId_PIPELINE_A: return &pBandSettings->pipeline_a;
  case RadioSettings_PipelineId_PIPELINE_B: return &pBandSettings->pipeline_b;
  default: return nullptr;
  }
}

[[nodiscard]] const RadioSettings_RxPipelineSettingsPb*
  RadioSettings::getFocusBandFocusRxPipelineSettings() const
{
  return getFocusRxPipelineSettings(static_cast<SplitBandId>(m_rawSettings.active_bands.focus_band_id));
}

void
RadioSettings::InitBandAndPipelineIdsWithDefaults()
{
  if (m_rawSettings.active_bands.focus_band_id == RadioSettings_SplitBandId_SPLIT_BAND_NONE) {
    m_rawSettings.active_bands.focus_band_id = RadioSettings_SplitBandId_SPLIT_BAND_ONE;
    m_rawSettings.active_bands.has_focus_band_id = true;
  }
  if (m_rawSettings.active_bands.rx_band_id == RadioSettings_SplitBandId_SPLIT_BAND_NONE) {
    m_rawSettings.active_bands.rx_band_id = RadioSettings_SplitBandId_SPLIT_BAND_ONE;
    m_rawSettings.active_bands.has_rx_band_id = true;
  }
  if (m_rawSettings.active_bands.tx_band_id == RadioSettings_SplitBandId_SPLIT_BAND_NONE) {
    m_rawSettings.active_bands.tx_band_id = RadioSettings_SplitBandId_SPLIT_BAND_ONE;
    m_rawSettings.active_bands.has_tx_band_id = true;
  }
  if (m_rawSettings.active_bands.band_1.focus_pipeline_id == RadioSettings_PipelineId_PIPELINE_NONE) {
    m_rawSettings.active_bands.band_1.focus_pipeline_id = RadioSettings_PipelineId_PIPELINE_A;
    m_rawSettings.active_bands.band_1.has_focus_pipeline_id = true;
  }
  if (m_rawSettings.active_bands.band_2.focus_pipeline_id == RadioSettings_PipelineId_PIPELINE_NONE) {
    m_rawSettings.active_bands.band_2.focus_pipeline_id = RadioSettings_PipelineId_PIPELINE_A;
    m_rawSettings.active_bands.band_2.has_focus_pipeline_id = true;
  }
  if (m_rawSettings.active_bands.band_1.tx_pipeline_id == RadioSettings_PipelineId_PIPELINE_NONE) {
    m_rawSettings.active_bands.band_1.tx_pipeline_id = RadioSettings_PipelineId_PIPELINE_A;
    m_rawSettings.active_bands.band_1.has_tx_pipeline_id = true;
  }
  if (m_rawSettings.active_bands.band_2.tx_pipeline_id == RadioSettings_PipelineId_PIPELINE_NONE) {
    m_rawSettings.active_bands.band_2.tx_pipeline_id = RadioSettings_PipelineId_PIPELINE_A;
    m_rawSettings.active_bands.band_2.has_tx_pipeline_id = true;
  }
}

ResultCode
RadioSettings::resolveIndirection(
  const SettingFieldPath& indirectPath,
  uint32_t startingAtIndex,
  SettingFieldPath& resolvedPath
  )
{
  if (startingAtIndex >= indirectPath.size()) {
    return ResultCode::ERR_SETTING_RESOLVE_INDIRECTION_PATH_INVALID;
  }
  switch (indirectPath[startingAtIndex]) {
  case RadioSettings_RadioSettingsPb_active_bands_tag:
    resolvedPath.emplace_back(RadioSettings_RadioSettingsPb_active_bands_tag);
    return resolveActiveBandsIndirection(m_rawSettings.active_bands, indirectPath, startingAtIndex + 1, resolvedPath);
  }
  for (uint32_t i = startingAtIndex; i < indirectPath.size(); i++) {
    resolvedPath.emplace_back(indirectPath[i]);
  }
  return ResultCode::OK;
}

ResultCode
RadioSettings::resolveActiveBandsIndirection(
  const RadioSettings_ActiveBandSettingsPb& rawActiveBands,
  const SettingFieldPath& indirectPath,
  uint32_t startingAtIndex,
  SettingFieldPath& resolvedPath
)
{
  if (startingAtIndex >= indirectPath.size()) {
    return ResultCode::ERR_SETTING_RESOLVE_INDIRECTION_PATH_INVALID;
  }
  switch (indirectPath[startingAtIndex]) {
  case RadioSettings_ActiveBandSettingsPb_focus_band_tag:
    {
      SplitBandId bandId = static_cast<SplitBandId>(rawActiveBands.focus_band_id);
      if (bandId == SplitBandId::One) {
        resolvedPath.emplace_back(RadioSettings_ActiveBandSettingsPb_band_1_tag);
        return resolveBandIndirection(rawActiveBands.band_1, indirectPath, startingAtIndex + 1, resolvedPath);
      }
      if (bandId == SplitBandId::Two) {
        resolvedPath.emplace_back(RadioSettings_ActiveBandSettingsPb_band_2_tag);
        return resolveBandIndirection(rawActiveBands.band_2, indirectPath, startingAtIndex + 1, resolvedPath);
      }
      return ResultCode::ERR_SETTING_BAND_SETTINGS_FOCUS_BAND_NOT_SET;
    }
    break;
  default: break;
  }
  for (uint32_t i = startingAtIndex; i < indirectPath.size(); i++) {
    resolvedPath.emplace_back(indirectPath[i]);
  }
  return ResultCode::OK;
}

ResultCode
RadioSettings::resolveBandIndirection(
  const RadioSettings_BandSettingsPb& rawBandSettings,
  const SettingFieldPath& indirectPath,
  uint32_t startingAtIndex,
  SettingFieldPath& resolvedPath
)
{
  if (startingAtIndex >= indirectPath.size()) {
    return ResultCode::ERR_SETTING_RESOLVE_INDIRECTION_PATH_INVALID;
  }
  uint32_t nextIndex = startingAtIndex;
  switch (indirectPath[startingAtIndex]) {
  case RadioSettings_BandSettingsPb_focus_pipeline_tag:
    {
      PipelineId pipelineId = static_cast<PipelineId>(rawBandSettings.focus_pipeline_id);
      if (pipelineId == PipelineId::A) {
        resolvedPath.emplace_back(RadioSettings_BandSettingsPb_pipeline_a_tag);
      } else if (pipelineId == PipelineId::B) {
        resolvedPath.emplace_back(RadioSettings_BandSettingsPb_pipeline_b_tag);
      } else {
        return ResultCode::ERR_SETTING_BAND_SETTINGS_FOCUS_PIPELINE_NOT_SET;
      }
      nextIndex++;
      return ResultCode::OK;
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