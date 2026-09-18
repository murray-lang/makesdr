#include "iq/pipeline/IqRxPipeline.h"

#define DEFAULT_SAMPLE_RATE 48000


IqRxPipeline::IqRxPipeline(const ModeList& modes)
  : IqPipeline(modes)
  , m_amDemodulator(*modes.findModeByType(makesdr_ModeType_MODE_AMN), DEFAULT_SAMPLE_RATE)
  , m_fmnDemodulator(*modes.findModeByType(makesdr_ModeType_MODE_FMN),DEFAULT_SAMPLE_RATE)
  , m_fmwDemodulator(*modes.findModeByType(makesdr_ModeType_MODE_FMW),DEFAULT_SAMPLE_RATE)
  , m_ssbDemodulator(*modes.findModeByType(makesdr_ModeType_MODE_USB),DEFAULT_SAMPLE_RATE)
  , m_cwDemodulator(*modes.findModeByType(makesdr_ModeType_MODE_CWU),DEFAULT_SAMPLE_RATE)
  , m_pDemodulator(nullptr)
  // , m_monitorStage(m_eventTargetProvider)
{
  // m_monitorStage.setSampleRateProvider([this]() -> uint32_t { return this->m_inputSampleRate; });
  appendStage(&m_iqCorrection);
  appendStage(&m_oscillatorMixer);
  appendStage(&m_ifFilter);
  appendStage(&m_resampler);

  m_pDemodulator = &m_ssbDemodulator; // For now TODO: Get setting updates working again.
}

void
IqRxPipeline::initialise(IqIo* pIo, AudioSink* pAudioSink)
{
  IqPipeline::initialise(pIo, pAudioSink);
  m_inputSampleRate = pIo->getInputSampleRate();
  m_oscillatorMixer.initialise(m_inputSampleRate, 0);
  uint32_t preferredOutputRate = pIo->getOutputSampleRate();
  setOutputSampleRate(preferredOutputRate);
}

void
IqRxPipeline::setOutputSampleRate(uint32_t preferredOutputRate)
{
  m_outputSampleRate = m_resampler.configure(m_inputSampleRate, preferredOutputRate);
  m_amDemodulator.setSampleRate(m_outputSampleRate);
  m_fmnDemodulator.setSampleRate(m_outputSampleRate);
  m_ssbDemodulator.setSampleRate(m_outputSampleRate);
}

uint32_t
IqRxPipeline::getMaxFramesPerInputPacket() const
{
  return PIPELINE_BUFFER_LENGTH / 2; // 2 is the number of channels (I + Q)
}

uint32_t
IqRxPipeline::getMaxFramesPerOutputPacket() const
{
  uint32_t inputMaxFrames = getMaxFramesPerInputPacket();
  if (m_inputSampleRate == 0) {
    return 0;
  }
  return inputMaxFrames * m_outputSampleRate / m_inputSampleRate;
}

ResultCode
IqRxPipeline::apply(const BandRfSettings* bandRfSettings, RxPipelineSettings* settings)
{
  ResultCode rc = IqPipeline::apply(bandRfSettings, &settings->base());
  if (rc != ResultCode::OK) return rc;
  if (settings->hasAgcSpeed()) {
    const AgcSpeed agcSpeed = settings->agcSpeed();
    lock_guard<mutex> lock(m_settingsMutex);
    // m_iqAgcStage.setSpeed(agcSpeed);
  }
  return ResultCode::OK;
}

void
IqRxPipeline::calcNyquistOffsetsLimits(int32_t* maxNegative, int32_t* maxPositive) const
{
  int32_t nyquist = static_cast<int32_t>(m_inputSampleRate) / 2;
  *maxNegative = -nyquist - m_mode.hi_cut;
  *maxPositive = nyquist - m_mode.lo_cut;
}

void
IqRxPipeline::setMode(const Mode& mode)
{

  IqPipeline::setMode(mode);
  // const uint32_t decimatorOutputRate = m_decimator.getOutputSampleRate();
  m_ifFilter.configure(mode.loCut(), mode.hiCut(), mode.offset(), m_inputSampleRate);
  setDemodulator(mode);
}

void
IqRxPipeline::setDemodulator(const Mode& mode)
{
  switch (mode.type()) {
  case Mode::Type::AMN:
  case Mode::Type::AMW:
    m_pDemodulator = &m_amDemodulator;
    break;
  case Mode::Type::FMN:
    m_pDemodulator = &m_fmnDemodulator;
    break;
  case Mode::Type::FMW:
    m_pDemodulator = &m_fmwDemodulator;
    break;
  case Mode::Type::USB:
  case Mode::Type::LSB:
    m_pDemodulator = &m_ssbDemodulator;
    break;
  case Mode::Type::CWU:
  case Mode::Type::CWL:
    m_pDemodulator = &m_cwDemodulator;
    break;
  default:
    m_pDemodulator = nullptr;
    // throw SettingsException("Unknown mode type");
    break;
  }
  if (m_pDemodulator != nullptr) {
    m_pDemodulator->setMode(mode.raw());
  }
}

uint32_t
IqRxPipeline::sinkIq(ComplexPingPongBuffers& samples, uint32_t length)
{
  uint32_t outputLength = length;

  lock_guard<mutex> lock(m_settingsMutex);
  for (auto stage : m_stages) {
    outputLength = stage->processSamples(samples, outputLength);
    samples.flip();
  }
  if (m_pDemodulator != nullptr) {
    outputLength = m_pDemodulator->processSamples(samples.input(), m_audioBuffer, outputLength);
    m_audioBuffer.resize(outputLength);
  } else {
    outputLength = 0;
  }
  if (outputLength > 0 && m_pAudioOutSink != nullptr) {
    m_pAudioOutSink->sinkAudio(m_audioBuffer, outputLength, m_pDemodulator->getNumOutputChannels());
  }
  return outputLength;
}