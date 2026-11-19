#include <settings/model/radio/iq/SingleIqRxUpdater.h>
#include <settings/model/update/SingleIqRxResolved.h>
#include <settings/model/update/FieldUpdate.h>

#include <settings/model/radio/PipelineId.h>
#include <settings/model/radio/SplitBandId.h>

#include <settings/model/meta/band/BandTypes.h>

using namespace SingleIqRxResolved;

ResultCode
SingleIqRxUpdater::selectBand(const char * bandName, bool final)
{
//active_bands_focus_band_band_request
  NameString bandNameString(bandName);
  return notifyFieldUpdate(
    FieldUpdate(active_bands_focus_band_band_request, bandNameString, FieldUpdateMeaning::VALUE, final));
}


ResultCode
SingleIqRxUpdater::ptt(bool on)
{
  return notifyFieldUpdate(FieldUpdate(::ptt, on, FieldUpdateMeaning::VALUE));
}


ResultCode
SingleIqRxUpdater::mutePipeline(SplitBandId bandId, PipelineId pipelineId, bool mute, bool final)
{
  return notifyFieldUpdate(FieldUpdate(active_bands_focus_band_focus_pipeline_mute, mute, FieldUpdateMeaning::VALUE, final));
}

ResultCode
SingleIqRxUpdater::setFocusMode(Mode::Type modeType, bool final)
{
  return notifyFieldUpdate(
    FieldUpdate(active_bands_focus_band_focus_pipeline_base_mode_request, modeType, FieldUpdateMeaning::VALUE, final)
  );
}

ResultCode
SingleIqRxUpdater::setFocusAgc(AgcSpeed agcSpeed, bool final)
{
  return notifyFieldUpdate(
    FieldUpdate(active_bands_focus_band_focus_pipeline_agc_speed, agcSpeed, FieldUpdateMeaning::VALUE, final)
  );
}

ResultCode
SingleIqRxUpdater::setCentreFrequency(int64_t frequency, bool final)
{
  return notifyFieldUpdate(
    FieldUpdate(active_bands_focus_band_rf_frequency, frequency, FieldUpdateMeaning::VALUE, final)
  );
}

ResultCode
SingleIqRxUpdater::stepCentreFrequency(int32_t steps, bool final)
{
  return notifyFieldUpdate(
    FieldUpdate(active_bands_focus_band_rf_frequency, steps, FieldUpdateMeaning::DELTA, final)
  );
}

ResultCode
SingleIqRxUpdater::setFocusPipelineFrequency(int64_t frequency, bool final)
{
  return notifyFieldUpdate(
    FieldUpdate(active_bands_focus_band_focus_pipeline_base_rf_frequency_value, frequency, FieldUpdateMeaning::VALUE, final)
  );
}

ResultCode
SingleIqRxUpdater::stepFocusPipelineFrequency(int32_t steps, bool final)
{
  FieldUpdate update (active_bands_focus_band_focus_pipeline_base_rf_frequency, steps, FieldUpdateMeaning::DELTA, final);
  return notifyFieldUpdate(update);
}
