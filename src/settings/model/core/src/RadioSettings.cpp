#include "settings/model/core/RadioSettings.h"

RadioSettings::RadioSettings(
  RadioSettings_RadioSettingsPb& raw,
  const RadioSettings_RadioMetaPb& meta,
  BandSettingsCache& cache
  )
    : m_rawSettings(raw)
    , m_visitor(&raw, &RadioSettings_RadioSettingsPb_msg)
    , m_meta(meta)
    , m_cache(cache)
  {
    InitBandAndPipelineIdsWithDefaults();
  }

ResultCode
RadioSettings::applySettingFieldUpdate(const SettingFieldUpdate &settingUpdate)
{
  ResultCode rc = ResultCode::OK;
  if (settingUpdate.isIndirect()) {
    rc = updateIndirectField(settingUpdate, 0);

  } else {
    rc = m_visitor.updateField(settingUpdate);
  }
  if (rc == ResultCode::OK && settingUpdate.trigger() != AutoCompleteTrigger::NONE) {
    return autoComplete(settingUpdate.path(), 0, settingUpdate.trigger());

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
RadioSettings::updateIndirectField(const SettingFieldUpdate &settingUpdate, uint32_t startingAtIndex)
{
  const SettingFieldPath& path = settingUpdate.path();
  if (path[startingAtIndex] == RadioSettings_RadioSettingsPb_active_bands_tag) {
    return updateIndirectActiveBandsField(m_rawSettings.active_bands, settingUpdate, startingAtIndex + 1);
  }
  return ResultCode::ERR_SETTING_INDIRECT_PATH_INVALID;
}

ResultCode
RadioSettings::updateIndirectActiveBandsField(
    RadioSettings_ActiveBandSettingsPb& rawActiveBands,
    const SettingFieldUpdate &settingUpdate,
    uint32_t startingAtIndex
    )
{
  const SettingFieldPath& path = settingUpdate.path();
  if (startingAtIndex >= path.size()) {
    return ResultCode::ERR_SETTING_INDIRECT_PATH_INVALID;
  }
  SplitBandId bandId = SplitBandId::None;
  switch (path[startingAtIndex]) {
  case RadioSettings_ActiveBandSettingsPb_focus_band_tag:
    bandId = static_cast<SplitBandId>(rawActiveBands.focus_band_id);
    break;
  case RadioSettings_ActiveBandSettingsPb_band_1_tag:
    bandId = SplitBandId::One;
    break;
  case RadioSettings_ActiveBandSettingsPb_band_2_tag:
    bandId = SplitBandId::Two;
    break;
  default: return ResultCode::ERR_SETTING_INDIRECT_PATH_INVALID;
  }
  ResultCode rc = ResultCode::OK;
  if (bandId == SplitBandId::One) {
    rc = updateIndirectBandField(rawActiveBands.band_1, settingUpdate, startingAtIndex + 1);
    if (rc == ResultCode::OK) {
      rawActiveBands.has_band_1 = true;
    }
  } else if (bandId == SplitBandId::Two) {
    rc = updateIndirectBandField(rawActiveBands.band_2, settingUpdate, startingAtIndex + 1);
    if (rc == ResultCode::OK) {
      rawActiveBands.has_band_2 = true;
    }
  } else {
    return ResultCode::ERR_SETTING_BAND_SETTINGS_FOCUS_BAND_NOT_SET;
  }
  if (rc == ResultCode::OK) {
    m_rawSettings.has_active_bands = true;
  }
  return rc;
}

ResultCode
RadioSettings::updateIndirectBandField(
    RadioSettings_BandSettingsPb& rawBandSettings,
    const SettingFieldUpdate &settingUpdate,
    uint32_t startingAtIndex
  )
{
  const SettingFieldPath& path = settingUpdate.path();
  if (startingAtIndex >= path.size()) {
    return ResultCode::ERR_SETTING_INDIRECT_PATH_INVALID;
  }

  PipelineId pipelineId = PipelineId::NONE;
  switch (path[startingAtIndex]) {
  case RadioSettings_BandSettingsPb_focus_pipeline_tag:
    pipelineId = static_cast<PipelineId>(rawBandSettings.focus_pipeline_id);
    break;
  case RadioSettings_BandSettingsPb_pipeline_a_tag:
    pipelineId = PipelineId::A;
    break;
  case RadioSettings_BandSettingsPb_pipeline_b_tag:
    pipelineId = PipelineId::B;
    break;
  default:
    return MessageVisitor::updateField(
      &rawBandSettings,
      &RadioSettings_BandSettingsPb_msg,
      settingUpdate,
      startingAtIndex + 1
      );
    break;
  }
  if (pipelineId == PipelineId::A) {
    return MessageVisitor::updateField(
      &rawBandSettings.pipeline_a,
      &RadioSettings_RxPipelineSettingsPb_msg,
      settingUpdate,
      startingAtIndex + 1
      );
  } if (pipelineId == PipelineId::B) {
    return MessageVisitor::updateField(
      &rawBandSettings.pipeline_b,
      &RadioSettings_RxPipelineSettingsPb_msg,
      settingUpdate,
      startingAtIndex + 1
      );
  }
  return ResultCode::ERR_SETTING_BAND_SETTINGS_FOCUS_PIPELINE_NOT_SET;
}

ResultCode
RadioSettings::autoComplete(const SettingFieldPath& path, uint32_t startingAtIndex, AutoCompleteTrigger trigger)
{
  if (startingAtIndex >= path.size()) {
    return ResultCode::ERR_SETTING_AUTOCOMPLETE_PATH_INVALID;
  }
  if (path[startingAtIndex] == RadioSettings_RadioSettingsPb_active_bands_tag) {
    return autoCompleteActiveBands(path, startingAtIndex + 1, trigger);
  }
  return ResultCode::ERR_SETTING_AUTOCOMPLETE_NOT_IMPLEMENTED;
}

ResultCode
RadioSettings::autoCompleteActiveBands(
  const SettingFieldPath& path,
  uint32_t startingAtIndex,
  AutoCompleteTrigger trigger
)
{
  if (startingAtIndex >= path.size()) {
    return ResultCode::ERR_SETTING_AUTOCOMPLETE_PATH_INVALID;
  }
  uint32_t nextTag = path[startingAtIndex];
  if (nextTag == RadioSettings_ActiveBandSettingsPb_focus_band_tag) {
    if (m_rawSettings.active_bands.focus_band_id == RadioSettings_SplitBandId_SPLIT_BAND_ONE) {
      nextTag = RadioSettings_ActiveBandSettingsPb_band_1_tag;
    } else if (m_rawSettings.active_bands.focus_band_id == RadioSettings_SplitBandId_SPLIT_BAND_TWO) {
      nextTag = RadioSettings_ActiveBandSettingsPb_band_2_tag;
    } else {
      return ResultCode::ERR_SETTING_BAND_SETTINGS_FOCUS_BAND_NOT_SET;
    }
  }
  switch (nextTag) {
  case RadioSettings_ActiveBandSettingsPb_band_1_tag:
    return autoCompleteBand(m_rawSettings.active_bands.band_1, path, startingAtIndex + 1, trigger);
  case RadioSettings_ActiveBandSettingsPb_band_2_tag:
    return autoCompleteBand(m_rawSettings.active_bands.band_2, path, startingAtIndex + 1, trigger);
  case RadioSettings_ActiveBandSettingsPb_is_split_tag:
    if (trigger == AutoCompleteTrigger::SPLIT_BAND) {
      return autoCompleteSplit(m_rawSettings.active_bands);
    }
  default: break;
  }
  return ResultCode::ERR_SETTING_AUTOCOMPLETE_NOT_IMPLEMENTED;
}

ResultCode
RadioSettings::autoCompleteBand(
  RadioSettings_BandSettingsPb& rawBandSettings,
  const SettingFieldPath& path,
  uint32_t startingAtIndex,
  AutoCompleteTrigger trigger
  )
{
  if (startingAtIndex >= path.size()) {
    return ResultCode::ERR_SETTING_AUTOCOMPLETE_PATH_INVALID;
  }
  switch (path[startingAtIndex]) {
  case RadioSettings_BandSettingsPb_band_request_tag:
    if (trigger == AutoCompleteTrigger::BAND) {
      return autoCompleteBandRequest(rawBandSettings);
    }
    break;
  case RadioSettings_BandSettingsPb_is_multi_pipeline_tag:
    if (trigger == AutoCompleteTrigger::MULTI_PIPELINE) {
      return autoCompleteMultiPipeline(rawBandSettings);
    }
    break;
  case RadioSettings_BandSettingsPb_pipeline_a_tag:
    return autoCompletePipeline(rawBandSettings.pipeline_a.base, path, startingAtIndex + 1, trigger);
    break;
  case RadioSettings_BandSettingsPb_pipeline_b_tag:
    return autoCompletePipeline(rawBandSettings.pipeline_b.base, path, startingAtIndex + 1, trigger);
    break;
  case RadioSettings_BandSettingsPb_tx_pipeline_tag:
    return autoCompletePipeline(rawBandSettings.tx_pipeline.base, path, startingAtIndex + 1, trigger);
    break;
  }
  return ResultCode::OK;
}

ResultCode
RadioSettings::autoCompleteSplit(RadioSettings_ActiveBandSettingsPb& rawActiveBandSettings)
{
  SplitBandId txId = static_cast<SplitBandId>(rawActiveBandSettings.tx_band_id);
  if (rawActiveBandSettings.is_split) {
    if (txId == SplitBandId::None) {
      if (rawActiveBandSettings.band_2.which_band_or_request == 0) {
        rawActiveBandSettings.tx_band_id = RadioSettings_SplitBandId_SPLIT_BAND_ONE;
        rawActiveBandSettings.has_tx_band_id = true;
        return ResultCode::OK;
      }
      ResultCode rc = ResultCode::OK;
      if (rawActiveBandSettings.band_2.which_band_or_request == RadioSettings_BandSettingsPb_band_request_tag) {
        rc = autoCompleteBandRequest(rawActiveBandSettings.band_2);
      } // Otherwise m_rawSettings.band_2.which_band_or_request == RadioSettings_BandSettingsPb_band_tag
      if (rc == ResultCode::OK) {
        rawActiveBandSettings.tx_band_id = RadioSettings_SplitBandId_SPLIT_BAND_TWO;
        rawActiveBandSettings.has_tx_band_id = true;
      }
    }
  } else {
    if (txId != SplitBandId::One) {
      rawActiveBandSettings.tx_band_id = RadioSettings_SplitBandId_SPLIT_BAND_ONE;
      rawActiveBandSettings.has_tx_band_id = true;
    }
    if (rawActiveBandSettings.focus_band_id != RadioSettings_SplitBandId_SPLIT_BAND_ONE) {
      rawActiveBandSettings.focus_band_id = RadioSettings_SplitBandId_SPLIT_BAND_ONE;
      rawActiveBandSettings.has_focus_band_id = true;
    }
    if (rawActiveBandSettings.rx_band_id != RadioSettings_SplitBandId_SPLIT_BAND_ONE) {
      rawActiveBandSettings.rx_band_id = RadioSettings_SplitBandId_SPLIT_BAND_ONE;
      rawActiveBandSettings.has_rx_band_id = true;
    }
  }
  return ResultCode::OK;
}

ResultCode
RadioSettings::autoCompleteBandRequest(RadioSettings_BandSettingsPb& rawBandSettings)
{

  if (rawBandSettings.which_band_or_request == RadioSettings_BandSettingsPb_band_request_tag) {
    if (!m_meta.hasBands()) {
      return ResultCode::ERR_SETTING_BAND_SETTINGS_NO_INFO;
    }
    // Try the cache first
    ResultCode rc = m_cache.get(&rawBandSettings);
    if (rc == ResultCode::OK) {
      // TODO: Set has_* = true on the band's fields
      rawBandSettings.which_band_or_request = RadioSettings_BandSettingsPb_band_tag;
      return rc;
    }
    // Not found in the cache. Get band info.
    const BandCategoryList& bandInfo = m_meta.bands();
    const RadioSettings_BandPb* pBand = bandInfo.findBand(rawBandSettings.band_or_request.band_request);
    if (pBand == nullptr) return ResultCode::ERR_SETTING_AUTOCOMPLETE_BAND_NOT_FOUND;

    rawBandSettings.band_or_request.band = *pBand;
    rawBandSettings.which_band_or_request = RadioSettings_BandSettingsPb_band_tag;

    applyBandDefaults(rawBandSettings);

    m_cache.set(&rawBandSettings); // This might fail due to being full, but ignore that.

    return ResultCode::OK;
  }
  return ResultCode::OK;
}

ResultCode
RadioSettings::autoCompleteMultiPipeline(RadioSettings_BandSettingsPb& rawBandSettings)
{
  if (rawBandSettings.is_multi_pipeline) {
    rawBandSettings.pipeline_b = rawBandSettings.pipeline_a;
    rawBandSettings.has_pipeline_b = true;
    rawBandSettings.tx_pipeline_id = RadioSettings_PipelineId_PIPELINE_B;
    rawBandSettings.has_tx_pipeline_id = true;
    rawBandSettings.tx_pipeline.base = rawBandSettings.pipeline_b.base; // Copy basics for tx tracking
    rawBandSettings.focus_pipeline_id = RadioSettings_PipelineId_PIPELINE_B;
    rawBandSettings.has_focus_pipeline_id = true;
    return ResultCode::OK;
  } else {
    rawBandSettings.tx_pipeline_id = RadioSettings_PipelineId_PIPELINE_A;
    rawBandSettings.has_tx_pipeline_id = true;
    rawBandSettings.tx_pipeline.base = rawBandSettings.pipeline_a.base; // Copy basics for tx tracking
    rawBandSettings.focus_pipeline_id = RadioSettings_PipelineId_PIPELINE_A;
    rawBandSettings.has_focus_pipeline_id = true;
  }
  return ResultCode::OK;
}

ResultCode
RadioSettings::autoCompletePipeline(
  RadioSettings_PipelineSettingsPb& rawPipelineSettings,
  const SettingFieldPath& path,
  uint32_t startingAtIndex,
  AutoCompleteTrigger trigger
)
{
  if (trigger == AutoCompleteTrigger::MODE) {
    if (startingAtIndex >= path.size()) {
      return ResultCode::ERR_SETTING_AUTOCOMPLETE_PATH_INVALID;
    }
    if (path[startingAtIndex] == RadioSettings_PipelineSettingsPb_mode_request_tag) {
      return autoCompleteMode(rawPipelineSettings);
    }
  }
  return ResultCode::OK;
}

ResultCode
RadioSettings::autoCompleteMode(RadioSettings_PipelineSettingsPb& rawPipelineSettings)
{
  if (rawPipelineSettings.which_mode_or_request == RadioSettings_PipelineSettingsPb_mode_request_tag) {
    if (!m_meta.hasModes()) return ResultCode::ERR_SETTING_BAND_SETTINGS_NO_INFO;

    const ModeList& modeList =  m_meta.modes();
    const RadioSettings_ModePb* pMode = modeList.findModeByType(rawPipelineSettings.mode_or_request.mode_request);
    if (pMode == nullptr) return ResultCode::ERR_SETTING_AUTOCOMPLETE_MODE_NOT_FOUND;

    rawPipelineSettings.mode_or_request.mode = *pMode;
    rawPipelineSettings.which_mode_or_request = RadioSettings_PipelineSettingsPb_mode_tag;
  }
  return ResultCode::OK;
}

ResultCode
RadioSettings::autoComplete()
{
  return autoComplete(m_rawSettings.active_bands);
}

ResultCode
RadioSettings::autoComplete(RadioSettings_ActiveBandSettingsPb& rawActiveBandSettings)
{
  ResultCode rc = autoComplete(rawActiveBandSettings.band_1);
  if (rc != ResultCode::OK) return rc;

  rc = autoComplete(rawActiveBandSettings.band_2);
  if (rc != ResultCode::OK) return rc;

  if (rawActiveBandSettings.has_is_split) {
    return autoCompleteSplit(rawActiveBandSettings);
  }
  return ResultCode::OK;
}

ResultCode
RadioSettings::autoComplete(RadioSettings_BandSettingsPb& rawBandSettings)
{
  ResultCode rc = ResultCode::OK;

  rc = autoComplete(rawBandSettings.pipeline_a.base);
  if (rc != ResultCode::OK) return rc;

  rc = autoComplete(rawBandSettings.pipeline_a.base);
  if (rc != ResultCode::OK) return rc;

  rc = autoComplete(rawBandSettings.tx_pipeline.base);
  if (rc != ResultCode::OK) return rc;

  if (rawBandSettings.which_band_or_request == RadioSettings_BandSettingsPb_band_request_tag) {
    rc = autoCompleteBandRequest(rawBandSettings);
    if (rc != ResultCode::OK) return rc;
  }
  if (rawBandSettings.has_is_multi_pipeline) {
    rc = autoCompleteMultiPipeline(rawBandSettings);
  }
  return rc;
}

ResultCode
RadioSettings::autoComplete(RadioSettings_PipelineSettingsPb& rawPipelineSettings)
{
  return autoCompleteMode(rawPipelineSettings);
}

ResultCode
RadioSettings::applyBandDefaults(RadioSettings_BandSettingsPb& rawBandSettings)
{
  ResultCode rc = applyBandDefaults(rawBandSettings.band_or_request.band, rawBandSettings.tx_pipeline.base);
  if (rc != ResultCode::OK) return rc;
  rc = applyBandDefaults(rawBandSettings.band_or_request.band, rawBandSettings.pipeline_a.base);
  if (rc != ResultCode::OK) return rc;
  return applyBandDefaults(rawBandSettings.band_or_request.band, rawBandSettings.pipeline_b.base);
}

ResultCode
RadioSettings::applyBandDefaults(const RadioSettings_BandPb& rawBand, RadioSettings_PipelineSettingsPb& rawPipeline)
{
  const RadioSettings_ModePb* pMode = m_meta.modes().findModeByType(rawBand.default_mode);
  if (pMode == nullptr) {
    return ResultCode::ERR_SETTING_MODE_TYPE_UNAVAILABLE;
  }
  rawPipeline.mode_or_request.mode = *pMode;
  rawPipeline.which_mode_or_request = RadioSettings_PipelineSettingsPb_mode_tag;
  return applyBandDefaults(rawBand, rawPipeline.rf);
}

ResultCode
RadioSettings::applyBandDefaults(const RadioSettings_BandPb& rawBand, RadioSettings_RfSettingsPb& rawRf)
{
  rawRf.has_centre_frequency = true;
  rawRf.has_vfo = true;
  rawRf.centre_frequency.value = rawBand.landing_frequency;
  rawRf.centre_frequency.coarse_delta = rawBand.default_coarse_step;
  rawRf.centre_frequency.fine_delta = rawBand.default_fine_step;
  rawRf.vfo.value = rawBand.landing_frequency;
  rawRf.vfo.coarse_delta = rawBand.default_coarse_step;
  rawRf.vfo.fine_delta =rawBand.default_fine_step;
  return ResultCode::OK;
}
