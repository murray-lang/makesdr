#include "settings/model/core/RadioSettings.h"

#include <stm32h745i/drivers/bsp/disco/stm32h745i_discovery.h>
#include <stm32h745i/app/support/safe_printf.h>

#include "settings/model/proto/ProtobufIo.h"


RadioSettings::RadioSettings(
  const makesdr_RadioMetaPb& meta,
  BandSettingsCache& cache
  )
  : m_assumeComplete(false)
  , m_payload(makesdr_RadioSettingsPayloadPb_init_zero)
  , m_visitor(&m_payload.body, &makesdr_RadioSettingsPb_msg)
  , m_meta(meta)
  , m_cache(cache)
{
  m_payload.header.payloadType = makesdr_RadioPayloadType_PAYLOAD_SETTINGS;
  m_payload.has_header = true;
  m_payload.has_body = true;
}

RadioSettings::RadioSettings(
  makesdr_RadioSettingsPb& raw,
  const makesdr_RadioMetaPb& meta,
  BandSettingsCache& cache
  )
  : m_assumeComplete(false)
  , m_payload(makesdr_RadioSettingsPayloadPb_init_zero)
  , m_visitor(&m_payload.body, &makesdr_RadioSettingsPb_msg)
  , m_meta(meta)
  , m_cache(cache)
{
  m_payload.header.payloadType = makesdr_RadioPayloadType_PAYLOAD_SETTINGS;
  m_payload.body = raw;
  m_payload.has_header = true;
  m_payload.has_body = true;
  InitBandAndPipelineIdsWithDefaults();
}

ResultCode
RadioSettings::writeProtobuf(
    makesdr_RadioPayloadPurpose purpose,
    uint8_t *buffer,
    size_t buffer_size,
    size_t* bytes_written
    )
{
  if (buffer_size < makesdr_RadioSettingsPayloadPb_size) {
    return ResultCode::ERR_PROTOBUF_BUFFER_TOO_SMALL;
  }
  makesdr_RadioPayloadPurpose savePurpose = m_payload.purpose;
  m_payload.purpose = purpose;
  ResultCode rc = ProtobufIo::writeProtobuf<makesdr_RadioSettingsPayloadPb>(
    buffer,
    makesdr_RadioSettingsPayloadPb_size,
    bytes_written,
    &makesdr_RadioSettingsPayloadPb_msg,
    m_payload
    );
  m_payload.purpose = savePurpose;
  return rc;
}

ResultCode
RadioSettings::readProtobuf(const uint8_t *buffer, size_t msg_length)
{
  return ProtobufIo::readProtobuf<makesdr_RadioSettingsPayloadPb>(
    buffer,
    msg_length,
    &makesdr_RadioSettingsPayloadPb_msg,
    makesdr_RadioSettingsPayloadPb_init_zero,
    m_payload
    );
}

void
RadioSettings::replace(makesdr_RadioSettingsPb& update, bool assumeComplete)
{
  m_payload.body = update;
  m_assumeComplete = assumeComplete;
  autoComplete();
};

ResultCode
RadioSettings::merge(const makesdr_RadioSettingsPb& update)
{
  ResultCode rc = m_visitor.mergePresentFields(&update);
  if (rc == ResultCode::OK) {
    return autoComplete();
  }
  return rc;
}

ResultCode
RadioSettings::setAllFieldsPresence(bool present)
{
  return m_visitor.setAllFieldsPresence(present);
}

void
RadioSettings::copyTo(makesdr_RadioSettingsPb& out) const
{
  out = m_payload.body;
}

ResultCode
RadioSettings::applySettingUpdate(const SettingUpdate &settingUpdate)
{

  ResultCode rc = ResultCode::OK;
  if (settingUpdate.isIndirect()) {
    rc = updateIndirectField(settingUpdate, 0);

  } else {
    const SettingPath& path = settingUpdate.path();
    // printf("[CM4]\tSettingPath: %lu, %lu, %lu, %lu, %lu, %lu\r\n", path[0], path[1], path[2], path[3], path[4], path[5]);
    rc = m_visitor.updateField(settingUpdate);
    // printf("[CM4]\tapplySettingUpdate: %d\r\n", rc);
  }
  if (rc == ResultCode::OK) {
    BSP_LED_Toggle(LED_GREEN);
  }
  if (rc == ResultCode::OK && settingUpdate.trigger() != AutoCompleteTrigger::NONE) {
    return autoComplete(settingUpdate.path(), 0, settingUpdate.trigger());

  }
  return rc;
}

ResultCode
RadioSettings::updateField(const SettingPath &path, const SettingUpdateVariant &value)
{
  return m_visitor.updateField(path, value);
}

ResultCode
RadioSettings::getField(const SettingPath &path, SettingUpdateVariant &value) const
{
  return m_visitor.getField(path, value);
}

ResultCode
RadioSettings::getField(
  const SettingPath &path,
  SettingUpdateVariant &value,
  bool mustHave,
  bool parentsMustHave,
  bool& retrieved
)
{
  return m_visitor.getField(path, value, mustHave, parentsMustHave, retrieved);
}

ResultCode
RadioSettings::setFieldPresence(const SettingPath &path, bool present)
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
  SettingPath &path,
  bool* isIndirectOut,
  AutoCompleteTrigger* triggerOut
)
{
  return MessageVisitor::resolveDottedPath(dottedPath, path, isIndirectOut, triggerOut);
}

[[nodiscard]] const makesdr_BandSettingsPb*
RadioSettings::getBandSettings(SplitBandId bandId) const
{
  switch (bandId)
  {
  case SplitBandId::One: return &m_payload.body.active_bands.band_1;
  case SplitBandId::Two: return &m_payload.body.active_bands.band_2;
  default: return nullptr;
  }
}

void
RadioSettings::ptt(bool on)
{
  m_payload.body.ptt = on;
  m_payload.body.has_ptt = true;
}

[[nodiscard]] PipelineId
RadioSettings::getFocusPipelineId(SplitBandId bandId) const
{
  switch (bandId) {
  case SplitBandId::One: return static_cast<PipelineId>(m_payload.body.active_bands.band_1.focus_pipeline_id);
  case SplitBandId::Two: return static_cast<PipelineId>(m_payload.body.active_bands.band_2.focus_pipeline_id);
  default: return PipelineId::NONE;
  }
}

[[nodiscard]] PipelineId
RadioSettings::getFocusBandFocusPipelineId() const
{
  switch (m_payload.body.active_bands.focus_band_id) {
  case makesdr_SplitBandId_SPLIT_BAND_ONE:
    return static_cast<PipelineId>(m_payload.body.active_bands.band_1.focus_pipeline_id);
  case makesdr_SplitBandId_SPLIT_BAND_TWO:
    return static_cast<PipelineId>(m_payload.body.active_bands.band_2.focus_pipeline_id);
  default: return PipelineId::NONE;
  }
}

[[nodiscard]] const makesdr_BandSettingsPb*
RadioSettings::getFocusBandSettings() const
{
  return getBandSettings(static_cast<SplitBandId>(m_payload.body.active_bands.focus_band_id));
}

[[nodiscard]] const makesdr_BandSettingsPb*
RadioSettings::getTxBandSettings() const
{
  return getBandSettings(static_cast<SplitBandId>(m_payload.body.active_bands.tx_band_id));
}

[[nodiscard]] const makesdr_RxPipelineSettingsPb*
  RadioSettings::getFocusRxPipelineSettings(SplitBandId bandId) const
{
  const makesdr_BandSettingsPb* pBandSettings = getBandSettings(bandId);
  if (pBandSettings == nullptr) return nullptr;
  switch (pBandSettings->focus_pipeline_id)
  {
    case makesdr_PipelineId_PIPELINE_A: return &pBandSettings->pipeline_a;
    case makesdr_PipelineId_PIPELINE_B: return &pBandSettings->pipeline_b;
    default: return nullptr;
  }
}

[[nodiscard]] const makesdr_RxPipelineSettingsPb*
RadioSettings::getTxPipelineSettings() const
{
  const makesdr_BandSettingsPb* pBandSettings = getTxBandSettings();
  if (pBandSettings == nullptr) return nullptr;
  switch (pBandSettings->tx_pipeline_id)
  {
  case makesdr_PipelineId_PIPELINE_A: return &pBandSettings->pipeline_a;
  case makesdr_PipelineId_PIPELINE_B: return &pBandSettings->pipeline_b;
  default: return nullptr;
  }
}

[[nodiscard]] const makesdr_RxPipelineSettingsPb*
  RadioSettings::getFocusBandFocusRxPipelineSettings() const
{
  return getFocusRxPipelineSettings(static_cast<SplitBandId>(m_payload.body.active_bands.focus_band_id));
}

void
RadioSettings::InitBandAndPipelineIdsWithDefaults()
{
  if (m_payload.body.active_bands.focus_band_id == makesdr_SplitBandId_SPLIT_BAND_NONE) {
    m_payload.body.active_bands.focus_band_id = makesdr_SplitBandId_SPLIT_BAND_ONE;
    m_payload.body.active_bands.has_focus_band_id = true;
  }
  if (m_payload.body.active_bands.rx_band_id == makesdr_SplitBandId_SPLIT_BAND_NONE) {
    m_payload.body.active_bands.rx_band_id = makesdr_SplitBandId_SPLIT_BAND_ONE;
    m_payload.body.active_bands.has_rx_band_id = true;
  }
  if (m_payload.body.active_bands.tx_band_id == makesdr_SplitBandId_SPLIT_BAND_NONE) {
    m_payload.body.active_bands.tx_band_id = makesdr_SplitBandId_SPLIT_BAND_ONE;
    m_payload.body.active_bands.has_tx_band_id = true;
  }
  if (m_payload.body.active_bands.band_1.focus_pipeline_id == makesdr_PipelineId_PIPELINE_NONE) {
    m_payload.body.active_bands.band_1.focus_pipeline_id = makesdr_PipelineId_PIPELINE_A;
    m_payload.body.active_bands.band_1.has_focus_pipeline_id = true;
  }
  if (m_payload.body.active_bands.band_2.focus_pipeline_id == makesdr_PipelineId_PIPELINE_NONE) {
    m_payload.body.active_bands.band_2.focus_pipeline_id = makesdr_PipelineId_PIPELINE_A;
    m_payload.body.active_bands.band_2.has_focus_pipeline_id = true;
  }
  if (m_payload.body.active_bands.band_1.tx_pipeline_id == makesdr_PipelineId_PIPELINE_NONE) {
    m_payload.body.active_bands.band_1.tx_pipeline_id = makesdr_PipelineId_PIPELINE_A;
    m_payload.body.active_bands.band_1.has_tx_pipeline_id = true;
  }
  if (m_payload.body.active_bands.band_2.tx_pipeline_id == makesdr_PipelineId_PIPELINE_NONE) {
    m_payload.body.active_bands.band_2.tx_pipeline_id = makesdr_PipelineId_PIPELINE_A;
    m_payload.body.active_bands.band_2.has_tx_pipeline_id = true;
  }
}

ResultCode
RadioSettings::updateIndirectField(const SettingUpdate &settingUpdate, uint32_t startingAtIndex)
{
  const SettingPath& path = settingUpdate.path();
  if (path[startingAtIndex] == makesdr_RadioSettingsPb_active_bands_tag) {
    return updateIndirectActiveBandsField(m_payload.body.active_bands, settingUpdate, startingAtIndex + 1);
  }
  return ResultCode::ERR_SETTING_INDIRECT_PATH_INVALID;
}

ResultCode
RadioSettings::updateIndirectActiveBandsField(
    makesdr_ActiveBandSettingsPb& rawActiveBands,
    const SettingUpdate &settingUpdate,
    uint32_t startingAtIndex
    )
{
  const SettingPath& path = settingUpdate.path();
  if (startingAtIndex >= path.size()) {
    return ResultCode::ERR_SETTING_INDIRECT_PATH_INVALID;
  }
  SplitBandId bandId = SplitBandId::None;
  switch (path[startingAtIndex]) {
  case makesdr_ActiveBandSettingsPb_focus_band_tag:
    bandId = static_cast<SplitBandId>(rawActiveBands.focus_band_id);
    break;
  case makesdr_ActiveBandSettingsPb_band_1_tag:
    bandId = SplitBandId::One;
    break;
  case makesdr_ActiveBandSettingsPb_band_2_tag:
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
    m_payload.body.has_active_bands = true;
  }
  return rc;
}

ResultCode
RadioSettings::updateIndirectBandField(
    makesdr_BandSettingsPb& rawBandSettings,
    const SettingUpdate &settingUpdate,
    uint32_t startingAtIndex
  )
{
  const SettingPath& path = settingUpdate.path();
  if (startingAtIndex >= path.size()) {
    return ResultCode::ERR_SETTING_INDIRECT_PATH_INVALID;
  }

  PipelineId pipelineId = PipelineId::NONE;
  switch (path[startingAtIndex]) {
  case makesdr_BandSettingsPb_focus_pipeline_tag:
    pipelineId = static_cast<PipelineId>(rawBandSettings.focus_pipeline_id);
    break;
  case makesdr_BandSettingsPb_pipeline_a_tag:
    pipelineId = PipelineId::A;
    break;
  case makesdr_BandSettingsPb_pipeline_b_tag:
    pipelineId = PipelineId::B;
    break;
  default:
    return MessageVisitor::updateField(
      &rawBandSettings,
      &makesdr_BandSettingsPb_msg,
      settingUpdate,
      startingAtIndex + 1
      );
    break;
  }
  if (pipelineId == PipelineId::A) {
    return MessageVisitor::updateField(
      &rawBandSettings.pipeline_a,
      &makesdr_RxPipelineSettingsPb_msg,
      settingUpdate,
      startingAtIndex + 1
      );
  } if (pipelineId == PipelineId::B) {
    return MessageVisitor::updateField(
      &rawBandSettings.pipeline_b,
      &makesdr_RxPipelineSettingsPb_msg,
      settingUpdate,
      startingAtIndex + 1
      );
  }
  return ResultCode::ERR_SETTING_BAND_SETTINGS_FOCUS_PIPELINE_NOT_SET;
}

ResultCode
RadioSettings::autoComplete(const SettingPath& path, uint32_t startingAtIndex, AutoCompleteTrigger trigger)
{
  if (startingAtIndex >= path.size()) {
    return ResultCode::ERR_SETTING_AUTOCOMPLETE_PATH_INVALID;
  }
  if (path[startingAtIndex] == makesdr_RadioSettingsPb_active_bands_tag) {
    return autoCompleteActiveBands(path, startingAtIndex + 1, trigger);
  }
  return ResultCode::ERR_SETTING_AUTOCOMPLETE_NOT_IMPLEMENTED;
}

ResultCode
RadioSettings::autoCompleteActiveBands(
  const SettingPath& path,
  uint32_t startingAtIndex,
  AutoCompleteTrigger trigger
)
{
  if (startingAtIndex >= path.size()) {
    return ResultCode::ERR_SETTING_AUTOCOMPLETE_PATH_INVALID;
  }
  uint32_t nextTag = path[startingAtIndex];
  if (nextTag == makesdr_ActiveBandSettingsPb_focus_band_tag) {
    if (m_payload.body.active_bands.focus_band_id == makesdr_SplitBandId_SPLIT_BAND_ONE) {
      nextTag = makesdr_ActiveBandSettingsPb_band_1_tag;
    } else if (m_payload.body.active_bands.focus_band_id == makesdr_SplitBandId_SPLIT_BAND_TWO) {
      nextTag = makesdr_ActiveBandSettingsPb_band_2_tag;
    } else {
      return ResultCode::ERR_SETTING_BAND_SETTINGS_FOCUS_BAND_NOT_SET;
    }
  }
  switch (nextTag) {
  case makesdr_ActiveBandSettingsPb_band_1_tag:
    return autoCompleteBand(m_payload.body.active_bands.band_1, path, startingAtIndex + 1, trigger);
  case makesdr_ActiveBandSettingsPb_band_2_tag:
    return autoCompleteBand(m_payload.body.active_bands.band_2, path, startingAtIndex + 1, trigger);
  case makesdr_ActiveBandSettingsPb_is_split_tag:
    if (trigger == AutoCompleteTrigger::SPLIT_BAND) {
      return autoCompleteSplit(m_payload.body.active_bands);
    }
  default: break;
  }
  return ResultCode::ERR_SETTING_AUTOCOMPLETE_NOT_IMPLEMENTED;
}

ResultCode
RadioSettings::autoCompleteBand(
  makesdr_BandSettingsPb& rawBandSettings,
  const SettingPath& path,
  uint32_t startingAtIndex,
  AutoCompleteTrigger trigger
  )
{
  if (startingAtIndex >= path.size()) {
    return ResultCode::ERR_SETTING_AUTOCOMPLETE_PATH_INVALID;
  }
  switch (path[startingAtIndex]) {
  case makesdr_BandSettingsPb_band_request_tag:
    if (trigger == AutoCompleteTrigger::BAND) {
      return autoCompleteBandRequest(rawBandSettings);
    }
    break;
  case makesdr_BandSettingsPb_is_multi_pipeline_tag:
    if (trigger == AutoCompleteTrigger::MULTI_PIPELINE) {
      return autoCompleteMultiPipeline(rawBandSettings);
    }
    break;
  case makesdr_BandSettingsPb_pipeline_a_tag:
    return autoCompletePipeline(rawBandSettings.pipeline_a.base, path, startingAtIndex + 1, trigger);
    break;
  case makesdr_BandSettingsPb_pipeline_b_tag:
    return autoCompletePipeline(rawBandSettings.pipeline_b.base, path, startingAtIndex + 1, trigger);
    break;
  case makesdr_BandSettingsPb_tx_pipeline_tag:
    return autoCompletePipeline(rawBandSettings.tx_pipeline.base, path, startingAtIndex + 1, trigger);
    break;
  }
  return ResultCode::OK;
}

ResultCode
RadioSettings::autoCompleteSplit(makesdr_ActiveBandSettingsPb& rawActiveBandSettings)
{
  SplitBandId txId = static_cast<SplitBandId>(rawActiveBandSettings.tx_band_id);
  if (rawActiveBandSettings.is_split) {
    if (txId == SplitBandId::None) {
      if (rawActiveBandSettings.band_2.which_band_or_request == 0) {
        rawActiveBandSettings.tx_band_id = makesdr_SplitBandId_SPLIT_BAND_ONE;
        rawActiveBandSettings.has_tx_band_id = true;
        return ResultCode::OK;
      }
      ResultCode rc = ResultCode::OK;
      if (rawActiveBandSettings.band_2.which_band_or_request == makesdr_BandSettingsPb_band_request_tag) {
        rc = autoCompleteBandRequest(rawActiveBandSettings.band_2);
      } // Otherwise m_payload.body.band_2.which_band_or_request == makesdr_BandSettingsPb_band_tag
      if (rc == ResultCode::OK) {
        rawActiveBandSettings.tx_band_id = makesdr_SplitBandId_SPLIT_BAND_TWO;
        rawActiveBandSettings.has_tx_band_id = true;
      }
    }
  } else {
    if (txId != SplitBandId::One) {
      rawActiveBandSettings.tx_band_id = makesdr_SplitBandId_SPLIT_BAND_ONE;
      rawActiveBandSettings.has_tx_band_id = true;
    }
    if (rawActiveBandSettings.focus_band_id != makesdr_SplitBandId_SPLIT_BAND_ONE) {
      rawActiveBandSettings.focus_band_id = makesdr_SplitBandId_SPLIT_BAND_ONE;
      rawActiveBandSettings.has_focus_band_id = true;
    }
    if (rawActiveBandSettings.rx_band_id != makesdr_SplitBandId_SPLIT_BAND_ONE) {
      rawActiveBandSettings.rx_band_id = makesdr_SplitBandId_SPLIT_BAND_ONE;
      rawActiveBandSettings.has_rx_band_id = true;
    }
  }
  return ResultCode::OK;
}

ResultCode
RadioSettings::autoCompleteBandRequest(makesdr_BandSettingsPb& rawBandSettings)
{

  if (rawBandSettings.which_band_or_request == makesdr_BandSettingsPb_band_request_tag) {
    if (!m_meta.hasBands()) {
      return ResultCode::ERR_SETTING_BAND_SETTINGS_NO_INFO;
    }
    // Try the cache first
    ResultCode rc = m_cache.get(&rawBandSettings);
    if (rc == ResultCode::OK) {
      // TODO: Set has_* = true on the band's fields
      rawBandSettings.which_band_or_request = makesdr_BandSettingsPb_band_tag;
      return rc;
    }
    // Not found in the cache. Get band info.
    const BandCategoryList& bandInfo = m_meta.bands();
    const makesdr_BandPb* pBand = bandInfo.findBand(rawBandSettings.band_or_request.band_request);
    if (pBand == nullptr) return ResultCode::ERR_SETTING_AUTOCOMPLETE_BAND_NOT_FOUND;

    rawBandSettings.band_or_request.band = *pBand;
    rawBandSettings.which_band_or_request = makesdr_BandSettingsPb_band_tag;

    applyBandDefaults(rawBandSettings);

    m_cache.set(&rawBandSettings); // This might fail due to being full, but ignore that.

    return ResultCode::OK;
  }
  return ResultCode::OK;
}

ResultCode
RadioSettings::autoCompleteMultiPipeline(makesdr_BandSettingsPb& rawBandSettings)
{
  if (rawBandSettings.is_multi_pipeline) {
    if (!rawBandSettings.has_pipeline_b) {
      rawBandSettings.pipeline_b = rawBandSettings.pipeline_a;
      rawBandSettings.has_pipeline_b = true;
    }
    if (!rawBandSettings.has_tx_pipeline_id) {
      rawBandSettings.tx_pipeline_id = makesdr_PipelineId_PIPELINE_B;
      rawBandSettings.has_tx_pipeline_id = true;
      rawBandSettings.tx_pipeline.base = rawBandSettings.pipeline_b.base; // Copy basics for tx tracking
    } else if (!rawBandSettings.has_tx_pipeline) {
      rawBandSettings.tx_pipeline.base =
        rawBandSettings.tx_pipeline_id == makesdr_PipelineId_PIPELINE_A ?
        rawBandSettings.pipeline_a.base : rawBandSettings.pipeline_b.base;
    }
    if (!rawBandSettings.has_focus_pipeline_id) {
      rawBandSettings.focus_pipeline_id = makesdr_PipelineId_PIPELINE_B;
      rawBandSettings.has_focus_pipeline_id = true;
    }
    return ResultCode::OK;
  } else {
    rawBandSettings.tx_pipeline_id = makesdr_PipelineId_PIPELINE_A;
    rawBandSettings.has_tx_pipeline_id = true;
    rawBandSettings.tx_pipeline.base = rawBandSettings.pipeline_a.base; // Copy basics for tx tracking
    rawBandSettings.focus_pipeline_id = makesdr_PipelineId_PIPELINE_A;
    rawBandSettings.has_focus_pipeline_id = true;
  }
  return ResultCode::OK;
}

ResultCode
RadioSettings::autoCompletePipeline(
  makesdr_PipelineSettingsPb& rawPipelineSettings,
  const SettingPath& path,
  uint32_t startingAtIndex,
  AutoCompleteTrigger trigger
)
{
  if (trigger == AutoCompleteTrigger::MODE) {
    if (startingAtIndex >= path.size()) {
      return ResultCode::ERR_SETTING_AUTOCOMPLETE_PATH_INVALID;
    }
    if (path[startingAtIndex] == makesdr_PipelineSettingsPb_mode_request_tag) {
      return autoCompleteMode(rawPipelineSettings);
    }
  }
  return ResultCode::OK;
}

ResultCode
RadioSettings::autoCompleteMode(makesdr_PipelineSettingsPb& rawPipelineSettings)
{
  if (rawPipelineSettings.which_mode_or_request == makesdr_PipelineSettingsPb_mode_request_tag) {
    if (!m_meta.hasModes()) return ResultCode::ERR_SETTING_BAND_SETTINGS_NO_INFO;

    const ModeList& modeList =  m_meta.modes();
    const makesdr_ModePb* pMode = modeList.findModeByType(rawPipelineSettings.mode_or_request.mode_request);
    if (pMode == nullptr) return ResultCode::ERR_SETTING_AUTOCOMPLETE_MODE_NOT_FOUND;

    rawPipelineSettings.mode_or_request.mode = *pMode;
    rawPipelineSettings.which_mode_or_request = makesdr_PipelineSettingsPb_mode_tag;
  }
  return ResultCode::OK;
}

ResultCode
RadioSettings::autoComplete()
{
  return autoComplete(m_payload.body.active_bands);
}

ResultCode
RadioSettings::autoComplete(makesdr_ActiveBandSettingsPb& rawActiveBandSettings)
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
RadioSettings::autoComplete(makesdr_BandSettingsPb& rawBandSettings)
{
  ResultCode rc = ResultCode::OK;

  rc = autoComplete(rawBandSettings.pipeline_a.base);
  if (rc != ResultCode::OK) return rc;

  rc = autoComplete(rawBandSettings.pipeline_a.base);
  if (rc != ResultCode::OK) return rc;

  rc = autoComplete(rawBandSettings.tx_pipeline.base);
  if (rc != ResultCode::OK) return rc;

  if (rawBandSettings.which_band_or_request == makesdr_BandSettingsPb_band_request_tag) {
    rc = autoCompleteBandRequest(rawBandSettings);
    if (rc != ResultCode::OK) return rc;
  }
  if (rawBandSettings.has_is_multi_pipeline) {
    rc = autoCompleteMultiPipeline(rawBandSettings);
  }
  return rc;
}

ResultCode
RadioSettings::autoComplete(makesdr_PipelineSettingsPb& rawPipelineSettings)
{
  return autoCompleteMode(rawPipelineSettings);
}

ResultCode
RadioSettings::applyBandDefaults(makesdr_BandSettingsPb& rawBandSettings)
{
  ResultCode rc = applyBandDefaults(rawBandSettings.band_or_request.band, rawBandSettings.tx_pipeline.base);
  if (rc != ResultCode::OK) return rc;
  rc = applyBandDefaults(rawBandSettings.band_or_request.band, rawBandSettings.pipeline_a.base);
  if (rc != ResultCode::OK) return rc;
  return applyBandDefaults(rawBandSettings.band_or_request.band, rawBandSettings.pipeline_b.base);
}

ResultCode
RadioSettings::applyBandDefaults(const makesdr_BandPb& rawBand, makesdr_PipelineSettingsPb& rawPipeline)
{
  const makesdr_ModePb* pMode = m_meta.modes().findModeByType(rawBand.default_mode);
  if (pMode == nullptr) {
    return ResultCode::ERR_SETTING_MODE_TYPE_UNAVAILABLE;
  }
  rawPipeline.mode_or_request.mode = *pMode;
  rawPipeline.which_mode_or_request = makesdr_PipelineSettingsPb_mode_tag;
  return applyBandDefaults(rawBand, rawPipeline.rf);
}

ResultCode
RadioSettings::applyBandDefaults(const makesdr_BandPb& rawBand, makesdr_RfSettingsPb& rawRf)
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
