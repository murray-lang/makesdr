#include <settings/model/radio/iq/DualIqRxUpdater.h>
#include <settings/model/update/DualIqRxResolved.h>
#include <settings/model/update/FieldUpdate.h>

#include <settings/model/radio/PipelineId.h>
#include <settings/model/radio/SplitBandId.h>

#include <settings/model/meta/band/BandTypes.h>

using namespace DualIqRxResolved;

ResultCode
DualIqRxUpdater::selectBand(const char * bandName, bool final)
{
  NameString bandNameString(bandName);
  return notifyFieldUpdate(
    FieldUpdate(active_bands_focus_band_band_request, bandNameString, FieldUpdateMeaning::VALUE, final));
}

ResultCode
DualIqRxUpdater::setMultiPipeline(SplitBandId bandId, bool isMulti, bool final)
{
  const FieldDescriptor& descriptor = active_bands_focus_band_is_multi_pipeline;
  return notifyFieldUpdate(FieldUpdate(descriptor, isMulti, FieldUpdateMeaning::VALUE, final));
}

ResultCode
DualIqRxUpdater::closePipeline(SplitBandId bandId, PipelineId pipelineId, bool final)
{
  if (pipelineId == PipelineId::NONE) return ResultCode::ERR_PIPELINE_ID_NONE;
  // Set the focus to the other pipeline
  const FieldDescriptor& descriptor = active_bands_focus_band_focus_pipeline_id;
  PipelineId pipelineIdToSet = pipelineId == PipelineId::A ? PipelineId::B : PipelineId::A;
  FieldUpdate focusUpdate(descriptor, static_cast<uint32_t>(pipelineIdToSet), FieldUpdateMeaning::VALUE, false);
  notifyFieldUpdate(focusUpdate);

  return notifyFieldUpdate(FieldUpdate(active_bands_focus_band_is_multi_pipeline, false, FieldUpdateMeaning::VALUE, final));
}

ResultCode
DualIqRxUpdater::ptt(bool on)
{
  return notifyFieldUpdate(FieldUpdate(::ptt, on, FieldUpdateMeaning::VALUE));
}

ResultCode
DualIqRxUpdater::setFocusPipeline(SplitBandId bandId, PipelineId pipelineId, bool final)
{
  if (pipelineId == PipelineId::NONE) return ResultCode::ERR_PIPELINE_ID_NONE;
  // Set the focus to the other pipeline
  const FieldDescriptor& descriptor = active_bands_focus_band_focus_pipeline_id;
  PipelineId pipelineIdToSet = pipelineId == PipelineId::A ? PipelineId::B : PipelineId::A;
  FieldUpdate focusUpdate(descriptor, static_cast<uint32_t>(pipelineIdToSet), FieldUpdateMeaning::VALUE, final);
  return notifyFieldUpdate(focusUpdate);
}

ResultCode
DualIqRxUpdater::mutePipeline(SplitBandId bandId, PipelineId pipelineId, bool mute, bool final)
{
  if (pipelineId == PipelineId::NONE) return ResultCode::ERR_PIPELINE_ID_NONE;
  return notifyFieldUpdate(FieldUpdate(active_bands_focus_band_focus_pipeline_mute, mute, FieldUpdateMeaning::VALUE, final));
}

ResultCode
DualIqRxUpdater::setFocusMode(Mode::Type modeType, bool final)
{
  return notifyFieldUpdate(
    FieldUpdate(active_bands_focus_band_focus_pipeline_base_mode_request, modeType, FieldUpdateMeaning::VALUE, final)
  );
}

ResultCode
DualIqRxUpdater::setFocusAgc(AgcSpeed agcSpeed, bool final)
{
  //active_bands_focus_band_focus_pipeline_agc_speed
  return notifyFieldUpdate(
    FieldUpdate(active_bands_focus_band_focus_pipeline_agc_speed, agcSpeed, FieldUpdateMeaning::VALUE, final)
  );
}

ResultCode
DualIqRxUpdater::setCentreFrequency(int64_t frequency, bool final)
{
  return notifyFieldUpdate(
    FieldUpdate(active_bands_focus_band_rf_frequency, frequency, FieldUpdateMeaning::VALUE, final)
  );
}

ResultCode
DualIqRxUpdater::stepCentreFrequency(int32_t steps, bool final)
{
  return notifyFieldUpdate(
    FieldUpdate(active_bands_focus_band_rf_frequency, steps, FieldUpdateMeaning::DELTA, final)
  );
}

ResultCode
DualIqRxUpdater::setFocusPipelineFrequency(int64_t frequency, bool final)
{
  return notifyFieldUpdate(
    FieldUpdate(active_bands_focus_band_focus_pipeline_base_rf_frequency_value, frequency, FieldUpdateMeaning::VALUE, final)
  );
}

ResultCode
DualIqRxUpdater::stepFocusPipelineFrequency(int32_t steps, bool final)
{
  FieldUpdate update (active_bands_focus_band_focus_pipeline_base_rf_frequency, steps, FieldUpdateMeaning::DELTA, final);
  return notifyFieldUpdate(update);
}
