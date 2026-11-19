#pragma once

#include <ResultCode.h>
#include <settings/model/radio/RadioSettingsUpdater.h>
#include <settings/model/update/FieldUpdateSource.h>
#include <settings/model/radio/SplitBandId.h>
#include <settings/model/radio/PipelineId.h>

class SingleIqRxUpdater : public RadioSettingsUpdater
{
public:
  SingleIqRxUpdater() = default;
  SingleIqRxUpdater(FieldUpdateSink* sink) : RadioSettingsUpdater(sink) {}
  ~SingleIqRxUpdater() override = default;

  ResultCode selectBand(const char * bandName, bool final) override;
  ResultCode ptt(bool on) override;
  ResultCode mutePipeline(SplitBandId bandId, PipelineId pipelineId, bool mute, bool final) override;
  ResultCode setFocusMode(Mode::Type modeType, bool final) override;
  ResultCode setFocusAgc(AgcSpeed agcSpeed, bool final) override;
  ResultCode setCentreFrequency(int64_t frequency, bool final) override;
  ResultCode stepCentreFrequency(int32_t steps, bool final) override;
  ResultCode setFocusPipelineFrequency(int64_t frequency, bool final) override;
  ResultCode stepFocusPipelineFrequency(int32_t steps, bool final) override;
};
