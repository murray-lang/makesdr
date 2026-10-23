#pragma once

#include <ResultCode.h>
#include <settings/model/radio/RadioSettingsUpdater.h>
#include <settings/model/update/FieldUpdateSource.h>
#include <settings/model/radio/SplitBandId.h>
#include <settings/model/radio/PipelineId.h>

class DualIqRxTxUpdater : public RadioSettingsUpdater
{
public:
  DualIqRxTxUpdater() = default;
  DualIqRxTxUpdater(FieldUpdateSink* sink) : RadioSettingsUpdater(sink) {}
  ~DualIqRxTxUpdater() override = default;

  ResultCode selectBand(const char * bandName, bool final) override;
  ResultCode setMultiPipeline(SplitBandId bandId, bool isMulti, bool final) override;
  ResultCode closePipeline(SplitBandId bandId, PipelineId pipelineId, bool final) override;
  ResultCode setTxPipeline(SplitBandId bandId, PipelineId pipelineId, bool final) override;
  ResultCode ptt(bool on) override;
  ResultCode setFocusPipeline(SplitBandId bandId, PipelineId pipelineId, bool final) override;
  ResultCode mutePipeline(SplitBandId bandId, PipelineId pipelineId, bool mute, bool final) override;
  ResultCode setFocusMode(Mode::Type modeType, bool final) override;
  ResultCode setFocusAgc(AgcSpeed agcSpeed, bool final) override;
  ResultCode setCentreFrequency(int64_t frequency, bool final) override;
  ResultCode stepCentreFrequency(int32_t steps, bool final) override;
  ResultCode setFocusPipelineFrequency(int64_t frequency, bool final) override;
  ResultCode stepFocusPipelineFrequency(int32_t steps, bool final) override;
};
