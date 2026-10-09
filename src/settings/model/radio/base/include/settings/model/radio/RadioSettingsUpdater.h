#pragma once

#include <ResultCode.h>
#include "IRadioSettingsUpdater.h"
#include <settings/model/update/FieldUpdateSource.h>

#include "SplitBandId.h"
#include "PipelineId.h"

class RadioSettingsUpdater : public IRadioSettingsUpdater, public FieldUpdateSource
{
public:
  RadioSettingsUpdater() : m_pSink(nullptr) {};
  RadioSettingsUpdater(FieldUpdateSink* sink)
    : m_pSink(nullptr)
  {
    RadioSettingsUpdater::connectFieldUpdateSink(sink);
  }
  ~RadioSettingsUpdater() override = default;

  ResultCode ptt(bool on) override { return ResultCode::ERR_UPDATE_NOT_IMPLEMENTED; }
  ResultCode selectBand(const char * bandName, bool final) override { return ResultCode::ERR_UPDATE_NOT_IMPLEMENTED; }
  ResultCode setMultiPipeline(SplitBandId bandId, bool isMulti, bool final) override { return ResultCode::ERR_UPDATE_NOT_IMPLEMENTED; }
  ResultCode closePipeline(SplitBandId bandId, PipelineId pipelineId, bool final) override { return ResultCode::ERR_UPDATE_NOT_IMPLEMENTED; }
  ResultCode setTxBand(SplitBandId bandId, bool final) override { return ResultCode::ERR_UPDATE_NOT_IMPLEMENTED; }
  ResultCode setTxPipeline(SplitBandId bandId, PipelineId pipelineId, bool final) override { return ResultCode::ERR_UPDATE_NOT_IMPLEMENTED; }
  ResultCode setFocusBand(SplitBandId bandId, bool final) override { return ResultCode::ERR_UPDATE_NOT_IMPLEMENTED; };
  ResultCode setFocusPipeline(SplitBandId bandId, PipelineId pipelineId, bool final) override { return ResultCode::ERR_UPDATE_NOT_IMPLEMENTED; };
  ResultCode mutePipeline(SplitBandId bandId, PipelineId pipelineId, bool mute, bool final) override { return ResultCode::ERR_UPDATE_NOT_IMPLEMENTED; };
  ResultCode split(bool final)  override { return ResultCode::ERR_UPDATE_NOT_IMPLEMENTED; };
  ResultCode unsplit(SplitBandId closeBandId, bool final) override { return ResultCode::ERR_UPDATE_NOT_IMPLEMENTED; }
  ResultCode setFocusMode(Mode::Type modeType, bool final) override { return ResultCode::ERR_UPDATE_NOT_IMPLEMENTED; }
  ResultCode setFocusAgc(AgcSpeed agcSpeed, bool final) override { return ResultCode::ERR_UPDATE_NOT_IMPLEMENTED; }
  ResultCode setCentreFrequency(int64_t frequency, bool final) override { return ResultCode::ERR_UPDATE_NOT_IMPLEMENTED; }
  ResultCode stepCentreFrequency(int32_t steps, bool final) override { return ResultCode::ERR_UPDATE_NOT_IMPLEMENTED; }
  ResultCode setFocusPipelineFrequency(int64_t frequency, bool final) override { return ResultCode::ERR_UPDATE_NOT_IMPLEMENTED; }
  ResultCode stepFocusPipelineFrequency(int32_t steps, bool final) override { return ResultCode::ERR_UPDATE_NOT_IMPLEMENTED; }

  void connectFieldUpdateSink(FieldUpdateSink* sink) override
  {
    m_pSink = sink;
  }
protected:

  ResultCode notifyFieldUpdate(const FieldUpdate& settingUpdate) override
  {
    if (m_pSink) {
      return m_pSink->applyFieldUpdate(settingUpdate);
    }
    return ResultCode::OK;
  }
  FieldUpdateSink* m_pSink;

};
