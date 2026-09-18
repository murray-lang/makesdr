#pragma once

#include <ResultCode.h>
#include <settings/model/radios/RadioSettingsUpdater.h>
#include <settings/model/message/FieldUpdateSource.h>
#include <settings/model/radios/SplitBandId.h>
#include <settings/model/radios/PipelineId.h>
#include <settings/model/message/StringRef.h>

class SplitBandDualIqRxTxUpdater : public RadioSettingsUpdater
{
public:
  SplitBandDualIqRxTxUpdater() = default;
  SplitBandDualIqRxTxUpdater(FieldUpdateSink* sink) : RadioSettingsUpdater(sink) {}
  ~SplitBandDualIqRxTxUpdater() override = default;

  ResultCode selectBand(const char * bandName) override;
  ResultCode setMultiPipeline(SplitBandId bandId, bool isMulti) override;
  ResultCode closePipeline(SplitBandId bandId, PipelineId pipelineId) override;
  ResultCode setTxBand(SplitBandId bandId) override;
  ResultCode setTxPipeline(SplitBandId bandId, PipelineId pipelineId) override;
  ResultCode ptt(bool on) override;
  ResultCode setFocusBand(SplitBandId bandId) override;
  ResultCode setFocusPipeline(SplitBandId bandId, PipelineId pipelineId) override;
  ResultCode mutePipeline(SplitBandId bandId, PipelineId pipelineId, bool mute) override;
  ResultCode split()  override;
  ResultCode unsplit(SplitBandId closeBandId) override;
  ResultCode setFocusMode(Mode::Type modeType) override;
  ResultCode setCentreFrequency(int64_t frequency) override;
  ResultCode stepCentreFrequency(int32_t steps) override;
  ResultCode setFocusPipelineFrequency(int64_t frequency) override;
  ResultCode stepFocusPipelineFrequency(int32_t steps) override;
};
