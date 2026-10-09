#include "settings/model/radio/iq/SplitBandDualIqRxTxUpdater.h"
#include <settings/model/update/SplitBandDualIqResolved.h>
#include <settings/model/update/FieldUpdate.h>

#include <settings/model/radio/PipelineId.h>
#include <settings/model/radio/SplitBandId.h>

#include <settings/model/meta/band/BandTypes.h>

ResultCode
SplitBandDualIqRxTxUpdater::selectBand(const char * bandName, bool final)
{
//active_bands_focus_band_band_request
  NameString bandNameString(bandName);
  return notifyFieldUpdate(
    FieldUpdate(active_bands_focus_band_band_request, bandNameString, FieldUpdateMeaning::VALUE, final));
}

ResultCode
SplitBandDualIqRxTxUpdater::setMultiPipeline(SplitBandId bandId, bool isMulti, bool final)
{
  if (bandId == SplitBandId::None) return ResultCode::ERR_BAND_ID_NONE;

  const FieldDescriptor& descriptor =
    bandId == SplitBandId::One ? active_bands_band_1_is_multi_pipeline : active_bands_band_2_is_multi_pipeline;

  return notifyFieldUpdate(FieldUpdate(descriptor, isMulti, FieldUpdateMeaning::VALUE, final));
}

ResultCode
SplitBandDualIqRxTxUpdater::closePipeline(SplitBandId bandId, PipelineId pipelineId, bool final)
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
    notifyFieldUpdate(FieldUpdate(active_bands_band_2_is_multi_pipeline, false, FieldUpdateMeaning::VALUE, final));
  }
  return ResultCode::OK;
}

ResultCode
SplitBandDualIqRxTxUpdater::setTxBand(SplitBandId bandId, bool final)
{
  if (bandId == SplitBandId::None) return ResultCode::ERR_BAND_ID_NONE;

  FieldUpdate update(active_bands_tx_band_id, static_cast<uint32_t>(bandId), FieldUpdateMeaning::VALUE, final);
  return notifyFieldUpdate(update);
}

ResultCode
SplitBandDualIqRxTxUpdater::setTxPipeline(SplitBandId bandId, PipelineId pipelineId, bool final)
{
  if (bandId == SplitBandId::None) return ResultCode::ERR_BAND_ID_NONE;
  if (pipelineId == PipelineId::NONE) return ResultCode::ERR_PIPELINE_ID_NONE;

  const FieldDescriptor& descriptor =
    bandId == SplitBandId::One ? active_bands_band_1_tx_pipeline_id : active_bands_band_2_tx_pipeline_id;

  return notifyFieldUpdate(
    FieldUpdate(descriptor, static_cast<uint32_t>(pipelineId), FieldUpdateMeaning::VALUE, final)
    );
}

ResultCode
SplitBandDualIqRxTxUpdater::ptt(bool on)
{
  return notifyFieldUpdate(FieldUpdate(::ptt, on, FieldUpdateMeaning::VALUE));
}

ResultCode
SplitBandDualIqRxTxUpdater::setFocusBand(SplitBandId bandId, bool final)
{
  if (bandId == SplitBandId::None) return ResultCode::ERR_BAND_ID_NONE;

  FieldUpdate update(active_bands_focus_band_id, static_cast<uint32_t>(bandId), FieldUpdateMeaning::VALUE, final);
  return notifyFieldUpdate(update);
}

ResultCode
SplitBandDualIqRxTxUpdater::setFocusPipeline(SplitBandId bandId, PipelineId pipelineId, bool final)
{
//active_bands_band_1_focus_pipeline_id
  if (bandId == SplitBandId::None) return ResultCode::ERR_BAND_ID_NONE;
  if (pipelineId == PipelineId::NONE) return ResultCode::ERR_PIPELINE_ID_NONE;

  const FieldDescriptor& descriptor =
    bandId == SplitBandId::One ? active_bands_band_1_focus_pipeline_id : active_bands_band_2_focus_pipeline_id;

  return notifyFieldUpdate(
    FieldUpdate(descriptor, static_cast<uint32_t>(pipelineId), FieldUpdateMeaning::VALUE, final)
    );
}

ResultCode
SplitBandDualIqRxTxUpdater::mutePipeline(SplitBandId bandId, PipelineId pipelineId, bool mute, bool final)
{
  if (bandId == SplitBandId::None) return ResultCode::ERR_BAND_ID_NONE;
  if (pipelineId == PipelineId::NONE) return ResultCode::ERR_PIPELINE_ID_NONE;
  const FieldDescriptor& descriptor = [](SplitBandId bandId, PipelineId pipelineId) -> const FieldDescriptor& {
    if (bandId == SplitBandId::One) {
      return pipelineId == PipelineId::A ? active_bands_band_1_pipeline_a_mute : active_bands_band_1_pipeline_b_mute;
    }
    return pipelineId == PipelineId::A ? active_bands_band_2_pipeline_a_mute : active_bands_band_2_pipeline_b_mute;
  }(bandId, pipelineId);
  return notifyFieldUpdate(FieldUpdate(descriptor, mute, FieldUpdateMeaning::VALUE, final));
}

ResultCode
SplitBandDualIqRxTxUpdater::split(bool final)
{
  return notifyFieldUpdate(FieldUpdate(active_bands_is_split, true, FieldUpdateMeaning::VALUE, final));
}

ResultCode
SplitBandDualIqRxTxUpdater::unsplit(SplitBandId closeBandId, bool final)
{
  SplitBandId keepBandId = closeBandId == SplitBandId::One ? SplitBandId::Two : SplitBandId::One;
  FieldUpdate update(active_bands_focus_band_id, static_cast<uint32_t>(keepBandId), FieldUpdateMeaning::VALUE, false);
  ResultCode rc = notifyFieldUpdate(update);

  if (rc != ResultCode::OK) return rc;
  return notifyFieldUpdate(FieldUpdate(active_bands_is_split, false, FieldUpdateMeaning::VALUE, final));
}

ResultCode
SplitBandDualIqRxTxUpdater::setFocusMode(Mode::Type modeType, bool final)
{
  return notifyFieldUpdate(
    FieldUpdate(active_bands_focus_band_focus_pipeline_base_mode_request, modeType, FieldUpdateMeaning::VALUE, final)
  );
}

ResultCode
SplitBandDualIqRxTxUpdater::setFocusAgc(AgcSpeed agcSpeed, bool final)
{
  //active_bands_focus_band_focus_pipeline_agc_speed
  return notifyFieldUpdate(
    FieldUpdate(active_bands_focus_band_focus_pipeline_agc_speed, agcSpeed, FieldUpdateMeaning::VALUE, final)
  );
}

ResultCode
SplitBandDualIqRxTxUpdater::setCentreFrequency(int64_t frequency, bool final)
{
  return notifyFieldUpdate(
    FieldUpdate(active_bands_focus_band_rf_frequency, frequency, FieldUpdateMeaning::VALUE, final)
  );
}

ResultCode
SplitBandDualIqRxTxUpdater::stepCentreFrequency(int32_t steps, bool final)
{
  return notifyFieldUpdate(
    FieldUpdate(active_bands_focus_band_rf_frequency, steps, FieldUpdateMeaning::DELTA, final)
  );
}

ResultCode
SplitBandDualIqRxTxUpdater::setFocusPipelineFrequency(int64_t frequency, bool final)
{
  return notifyFieldUpdate(
    FieldUpdate(active_bands_focus_band_focus_pipeline_base_rf_frequency_value, frequency, FieldUpdateMeaning::VALUE, final)
  );
}

ResultCode
SplitBandDualIqRxTxUpdater::stepFocusPipelineFrequency(int32_t steps, bool final)
{
  FieldUpdate update (active_bands_focus_band_focus_pipeline_base_rf_frequency, steps, FieldUpdateMeaning::DELTA, final);
  return notifyFieldUpdate(update);
}
