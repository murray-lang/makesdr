#include "settings/model/radios/iq/SplitBandDualIqRxTxUpdater.h"
#include <settings/model/path/SplitBandDualIqResolved.h>
#include <settings/model/message/FieldUpdate.h>

#include <settings/model/radios/PipelineId.h>
#include <settings/model/radios/SplitBandId.h>

#include <settings/model/data/band/BandTypes.h>

ResultCode
SplitBandDualIqRxTxUpdater::selectBand(const char * bandName)
{
//active_bands_focus_band_band_request
  NameString bandNameString(bandName);
  return notifyFieldUpdate(FieldUpdate(active_bands_focus_band_band_request, bandNameString, FieldUpdateMeaning::VALUE));
}

ResultCode
SplitBandDualIqRxTxUpdater::setMultiPipeline(SplitBandId bandId, bool isMulti)
{
  if (bandId == SplitBandId::None) return ResultCode::ERR_BAND_ID_NONE;

  const FieldDescriptor& descriptor =
    bandId == SplitBandId::One ? active_bands_band_1_is_multi_pipeline : active_bands_band_2_is_multi_pipeline;

  return notifyFieldUpdate(FieldUpdate(descriptor, isMulti, FieldUpdateMeaning::VALUE));
}

ResultCode
SplitBandDualIqRxTxUpdater::closePipeline(SplitBandId bandId, PipelineId pipelineId)
{
  if (bandId == SplitBandId::None) return ResultCode::ERR_BAND_ID_NONE;
  if (pipelineId == PipelineId::NONE) return ResultCode::ERR_PIPELINE_ID_NONE;

  if (bandId == SplitBandId::One) {
    const FieldDescriptor& descriptor = active_bands_band_1_focus_pipeline_id;
    PipelineId pipelineIdToSet = pipelineId == PipelineId::A ? PipelineId::B : PipelineId::A;
    FieldUpdate focusUpdate(descriptor, static_cast<uint32_t>(pipelineIdToSet), FieldUpdateMeaning::VALUE, false);

    notifyFieldUpdate(focusUpdate); // Not final
    notifyFieldUpdate(
      FieldUpdate(active_bands_band_1_is_multi_pipeline, false, FieldUpdateMeaning::VALUE, true));
  } else {
    const FieldDescriptor& descriptor = active_bands_band_2_focus_pipeline_id;
    PipelineId pipelineIdToSet = pipelineId == PipelineId::A ? PipelineId::B : PipelineId::A;
    FieldUpdate focusUpdate(descriptor, static_cast<uint32_t>(pipelineIdToSet), FieldUpdateMeaning::VALUE, false);

    notifyFieldUpdate(focusUpdate); // Not final
    notifyFieldUpdate(FieldUpdate(active_bands_band_2_is_multi_pipeline, false, FieldUpdateMeaning::VALUE, true));
  }
  return ResultCode::OK;
}

ResultCode
SplitBandDualIqRxTxUpdater::setTxBand(SplitBandId bandId)
{
  if (bandId == SplitBandId::None) return ResultCode::ERR_BAND_ID_NONE;

  FieldUpdate update(active_bands_tx_band_id, static_cast<uint32_t>(bandId), FieldUpdateMeaning::VALUE);
  return notifyFieldUpdate(update);
}

ResultCode
SplitBandDualIqRxTxUpdater::setTxPipeline(SplitBandId bandId, PipelineId pipelineId)
{
  if (bandId == SplitBandId::None) return ResultCode::ERR_BAND_ID_NONE;
  if (pipelineId == PipelineId::NONE) return ResultCode::ERR_PIPELINE_ID_NONE;

  const FieldDescriptor& descriptor =
    bandId == SplitBandId::One ? active_bands_band_1_tx_pipeline_id : active_bands_band_2_tx_pipeline_id;

  return notifyFieldUpdate(FieldUpdate(descriptor, static_cast<uint32_t>(pipelineId), FieldUpdateMeaning::VALUE));
}

ResultCode
SplitBandDualIqRxTxUpdater::ptt(bool on)
{
  return notifyFieldUpdate(FieldUpdate(::ptt, on, FieldUpdateMeaning::VALUE));
}

ResultCode
SplitBandDualIqRxTxUpdater::setFocusBand(SplitBandId bandId)
{
  if (bandId == SplitBandId::None) return ResultCode::ERR_BAND_ID_NONE;

  FieldUpdate update(active_bands_focus_band_id, static_cast<uint32_t>(bandId), FieldUpdateMeaning::VALUE);
  return notifyFieldUpdate(update);
}

ResultCode
SplitBandDualIqRxTxUpdater::setFocusPipeline(SplitBandId bandId, PipelineId pipelineId)
{
//active_bands_band_1_focus_pipeline_id
  if (bandId == SplitBandId::None) return ResultCode::ERR_BAND_ID_NONE;
  if (pipelineId == PipelineId::NONE) return ResultCode::ERR_PIPELINE_ID_NONE;

  const FieldDescriptor& descriptor =
    bandId == SplitBandId::One ? active_bands_band_1_focus_pipeline_id : active_bands_band_2_focus_pipeline_id;

  return notifyFieldUpdate(FieldUpdate(descriptor, static_cast<uint32_t>(pipelineId), FieldUpdateMeaning::VALUE));
}

ResultCode
SplitBandDualIqRxTxUpdater::mutePipeline(SplitBandId bandId, PipelineId pipelineId, bool mute)
{
  if (bandId == SplitBandId::None) return ResultCode::ERR_BAND_ID_NONE;
  if (pipelineId == PipelineId::NONE) return ResultCode::ERR_PIPELINE_ID_NONE;
  const FieldDescriptor& descriptor = [](SplitBandId bandId, PipelineId pipelineId) -> const FieldDescriptor& {
    if (bandId == SplitBandId::One) {
      return pipelineId == PipelineId::A ? active_bands_band_1_pipeline_a_mute : active_bands_band_1_pipeline_b_mute;
    }
    return pipelineId == PipelineId::A ? active_bands_band_2_pipeline_a_mute : active_bands_band_2_pipeline_b_mute;
  }(bandId, pipelineId);
  return notifyFieldUpdate(FieldUpdate(descriptor, mute, FieldUpdateMeaning::VALUE));
}

ResultCode
SplitBandDualIqRxTxUpdater::split()
{
  return notifyFieldUpdate(FieldUpdate(active_bands_is_split, true, FieldUpdateMeaning::VALUE));
}

ResultCode
SplitBandDualIqRxTxUpdater::unsplit(SplitBandId closeBandId)
{
  SplitBandId keepBandId = closeBandId == SplitBandId::One ? SplitBandId::Two : SplitBandId::One;
  ResultCode rc = setFocusBand(keepBandId);
  if (rc != ResultCode::OK) return rc;
  return notifyFieldUpdate(FieldUpdate(active_bands_is_split, false, FieldUpdateMeaning::VALUE));
}

ResultCode
SplitBandDualIqRxTxUpdater::setFocusMode(Mode::Type modeType)
{
  return notifyFieldUpdate(
    FieldUpdate(active_bands_focus_band_focus_pipeline_base_mode_request, modeType, FieldUpdateMeaning::VALUE)
  );
}

ResultCode
SplitBandDualIqRxTxUpdater::setCentreFrequency(int64_t frequency)
{
  return notifyFieldUpdate(
    FieldUpdate(active_bands_focus_band_rf_frequency, frequency, FieldUpdateMeaning::VALUE)
  );
}

ResultCode
SplitBandDualIqRxTxUpdater::stepCentreFrequency(int32_t steps)
{
  return notifyFieldUpdate(
    FieldUpdate(active_bands_focus_band_rf_frequency, steps, FieldUpdateMeaning::DELTA)
  );
}

ResultCode
SplitBandDualIqRxTxUpdater::setFocusPipelineFrequency(int64_t frequency)
{
  return notifyFieldUpdate(
    FieldUpdate(active_bands_focus_band_focus_pipeline_base_rf_frequency, frequency, FieldUpdateMeaning::VALUE)
  );
}

ResultCode
SplitBandDualIqRxTxUpdater::stepFocusPipelineFrequency(int32_t steps)
{
  FieldUpdate update (active_bands_focus_band_focus_pipeline_base_rf_frequency, steps, FieldUpdateMeaning::DELTA);
  return notifyFieldUpdate(update);
}
