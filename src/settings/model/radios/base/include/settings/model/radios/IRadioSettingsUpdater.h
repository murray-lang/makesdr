#pragma once
#include <ResultCode.h>
#include "SplitBandId.h"
#include "PipelineId.h"
#include "Mode.h"

class IRadioSettingsUpdater
{
public:
  virtual ~IRadioSettingsUpdater() = default;
  virtual ResultCode ptt(bool on) = 0;
  virtual ResultCode selectBand(const char* bandName) = 0;
  virtual ResultCode setMultiPipeline(SplitBandId bandId, bool isMulti) = 0;
  virtual ResultCode closePipeline(SplitBandId bandId, PipelineId pipelineId) = 0;
  virtual ResultCode setTxBand(SplitBandId bandId) = 0;
  virtual ResultCode setTxPipeline(SplitBandId bandId, PipelineId pipelineId) = 0;
  virtual ResultCode setFocusBand(SplitBandId bandId) = 0;
  virtual ResultCode setFocusPipeline(SplitBandId bandId, PipelineId pipelineId) = 0;
  virtual ResultCode mutePipeline(SplitBandId bandId, PipelineId pipelineId, bool mute) = 0;
  virtual ResultCode split() = 0;
  virtual ResultCode unsplit(SplitBandId closeBandId) = 0;
  virtual ResultCode setFocusMode(Mode::Type modeType) = 0;
  virtual ResultCode setCentreFrequency(int64_t frequency) = 0;
  virtual ResultCode stepCentreFrequency(int32_t steps) = 0;
  virtual ResultCode setFocusPipelineFrequency(int64_t frequency) = 0;
  virtual ResultCode stepFocusPipelineFrequency(int32_t steps) = 0;

};