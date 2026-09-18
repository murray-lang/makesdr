#include <CrossPlatformTypes.h>
#include "iq/pipeline/IqPipeline.h"

class ModeSettings;

IqPipeline::IqPipeline(const ModeList& modes)
  : m_modes(modes)
  , m_mode{}
  , m_inputSampleRate(0)
  , m_outputSampleRate(0)
  , m_pAudioOutSink(nullptr)
{
}

ResultCode
IqPipeline::apply(const BandRfSettings* bandRfSettings, PipelineSettings* pipelineSettings)
{
  if (pipelineSettings != nullptr && bandRfSettings != nullptr) {
    lock_guard<mutex> lock(m_settingsMutex);
    bool bandHasFrequency = bandRfSettings->hasFrequency();
    if (pipelineSettings->hasRfSettings() || bandHasFrequency) {
      PipelineRfSettings& rfSettings = pipelineSettings->rfSettings();
      if (rfSettings.hasFrequency() || bandHasFrequency) {
        applyNyquistLimits(bandRfSettings, &rfSettings);
        setOscillatorMixerFrequency(bandRfSettings, &rfSettings);
      }
    }
    if (pipelineSettings->isMode()) {
      setMode(pipelineSettings->mode());
    }
  }
  return ResultCode::OK;
}

void
IqPipeline::setOscillatorMixerFrequency(const BandRfSettings* bandRfSettings, const PipelineRfSettings* rfSettings)
{
  int64_t centreFreq = bandRfSettings->frequency();
  int64_t vfo = rfSettings->frequency();
  auto offset = static_cast<int32_t>(centreFreq - vfo);
  m_oscillatorMixer.setFrequency(offset);
}

