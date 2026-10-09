#pragma once
#include <ResultCode.h>
#include "AgcSpeed.h"
#include "SplitBandId.h"
#include "PipelineId.h"
#include "Mode.h"

class IRadioSettingsUpdater
{
public:
  virtual ~IRadioSettingsUpdater() = default;
  virtual ResultCode ptt(bool on) = 0;
  virtual ResultCode selectBand(const char* bandName, bool final) = 0;
  virtual ResultCode setMultiPipeline(SplitBandId bandId, bool isMulti, bool final) = 0;
  virtual ResultCode closePipeline(SplitBandId bandId, PipelineId pipelineId, bool final) = 0;
  virtual ResultCode setTxBand(SplitBandId bandId, bool final) = 0;
  virtual ResultCode setTxPipeline(SplitBandId bandId, PipelineId pipelineId, bool final) = 0;
  virtual ResultCode setFocusBand(SplitBandId bandId, bool final) = 0;
  virtual ResultCode setFocusPipeline(SplitBandId bandId, PipelineId pipelineId, bool final) = 0;
  virtual ResultCode mutePipeline(SplitBandId bandId, PipelineId pipelineId, bool mute, bool final) = 0;
  virtual ResultCode split(bool final) = 0;
  virtual ResultCode unsplit(SplitBandId closeBandId, bool final) = 0;
  virtual ResultCode setFocusMode(Mode::Type modeType, bool final) = 0;
  virtual ResultCode setFocusAgc(AgcSpeed agcSpeed, bool final) = 0;
  virtual ResultCode setCentreFrequency(int64_t frequency, bool final) = 0;
  virtual ResultCode stepCentreFrequency(int32_t steps, bool final) = 0;
  virtual ResultCode setFocusPipelineFrequency(int64_t frequency, bool final) = 0;
  virtual ResultCode stepFocusPipelineFrequency(int32_t steps, bool final) = 0;

};