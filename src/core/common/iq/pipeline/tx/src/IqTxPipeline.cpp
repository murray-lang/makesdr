#include "iq/pipeline/IqTxPipeline.h"

#define DEFAULT_SAMPLE_RATE 48000

IqTxPipeline::IqTxPipeline(const ModeList& modes)
: IqPipeline(modes)
  , m_fmnModulator(*modes.findModeByType(makesdr_ModeType_MODE_FMN),DEFAULT_SAMPLE_RATE)
  , m_fmwModulator(*modes.findModeByType(makesdr_ModeType_MODE_FMW),DEFAULT_SAMPLE_RATE)
  , m_ssbModulator(*modes.findModeByType(makesdr_ModeType_MODE_USB),DEFAULT_SAMPLE_RATE)
  , m_cwModulator(*modes.findModeByType(makesdr_ModeType_MODE_CWU),DEFAULT_SAMPLE_RATE)
  , m_pModulator(nullptr)
{
  appendStage(&m_ifFilter);
  appendStage(&m_resampler);
  appendStage(&m_oscillatorMixer);
  appendStage(&m_iqCorrection);
}

void
IqTxPipeline::initialise(IqIo* pIo, AudioSink* pAudioSink)
{
  IqPipeline::initialise(pIo, pAudioSink);
  m_inputSampleRate = pIo->getInputSampleRate();
  setModulatorSampleRate(m_inputSampleRate);
  uint32_t preferredOutputRate = pIo->getOutputSampleRate();
  setOutputSampleRate(preferredOutputRate);
  m_resampler.configure(m_inputSampleRate, m_outputSampleRate);

}

void
IqTxPipeline::setOutputSampleRate(uint32_t outputSampleRate)
{
  m_outputSampleRate = outputSampleRate;
  m_oscillatorMixer.initialise(outputSampleRate, 0);
}

uint32_t
IqTxPipeline::sinkIq(ComplexPingPongBuffers& samples, uint32_t length)
{
  uint32_t outputLength = 0;
  lock_guard<mutex> lock(m_settingsMutex);
  if (m_pModulator) {
    // if (m_twoToneSignal.getEnabled()) {
    //   outputLength = m_twoToneSignal.processSamples(samples, m_buffers.input(), length);
    // } else {
      outputLength = m_pModulator->processSamples(samples.input(), samples.output(), length);
      samples.flip();
    // }
  }
  if (outputLength > 0) {
    for (auto stage : m_stages) {
      outputLength = stage->processSamples(samples, outputLength);
      samples.flip();
    }
  }
  //  outputLength = length;
  if (outputLength > 0 && m_pAudioOutSink != nullptr) {
    uint32_t numReals = interleaveComplexToReal(samples.input(), m_audioBuffer, outputLength);
    // uint32_t numReals = interleaveComplexToReal(samples, m_audioBuffer, outputLength);

    m_pAudioOutSink->sinkAudio(m_audioBuffer, numReals, 2);
  }
  return outputLength;
}

ResultCode
IqTxPipeline::apply(const BandRfSettings* bandRfSettings, PipelineSettings* settings)
{
  return IqPipeline::apply(bandRfSettings, settings);
}

[[nodiscard]] uint32_t
IqTxPipeline::getMaxFramesPerInputPacket() const
{
  uint32_t outputMaxFrames = getMaxFramesPerOutputPacket();
  return outputMaxFrames * m_inputSampleRate / m_outputSampleRate;
}

[[nodiscard]] uint32_t
IqTxPipeline::getMaxFramesPerOutputPacket() const
{
  return PIPELINE_BUFFER_LENGTH / 2; // 2 is the number of channels (I + Q)

}

void
IqTxPipeline::calcNyquistOffsetsLimits(int32_t* maxNegative, int32_t* maxPositive) const
{
  int32_t nyquist = static_cast<int32_t>(m_outputSampleRate) / 2;
  *maxNegative = -nyquist - m_mode.lo_cut;
  *maxPositive = nyquist - m_mode.hi_cut;
}

void
IqTxPipeline::setMode(const Mode& mode)
{
  IqPipeline::setMode(mode);
  m_ifFilter.configure(mode.loCut(), mode.hiCut(), mode.offset(), m_inputSampleRate * 2);
  setModulator(mode);
}

void
IqTxPipeline::setModulator(const Mode& mode)
{
  switch (mode.type()) {
  case Mode::Type::AMN:
  case Mode::Type::AMW:
    m_pModulator = nullptr;
    break;
  case Mode::Type::FMN:
    m_pModulator = &m_fmnModulator;
    break;
  case Mode::Type::FMW:
    m_pModulator = &m_fmwModulator;
    break;
  case Mode::Type::USB:
  case Mode::Type::LSB:
    m_pModulator = &m_ssbModulator;
    break;
  case Mode::Type::CWU:
  case Mode::Type::CWL:
    m_pModulator = &m_cwModulator;
    break;
  default:
    m_pModulator = nullptr;
    // throw SettingsException("Unknown mode type");
    break;
  }
  if (m_pModulator != nullptr) {
    m_pModulator->setMode(mode.raw());
  }
}

void
IqTxPipeline::setModulatorSampleRate(uint32_t sampleRate)
{
  m_ssbModulator.setSampleRate(sampleRate);
  m_cwModulator.setSampleRate(sampleRate);
}

uint32_t
IqTxPipeline::interleaveComplexToReal(const ComplexSamplesMax& vcomplex, RealSamplesMax& vreal, uint32_t numComplexes)
{
  vreal.resize(numComplexes * 2);

  constexpr bool swapIQ = false;

  const auto* src = vcomplex.data();
  auto* dst = vreal.data();

  if constexpr (swapIQ) {
    for (uint32_t i = 0; i < numComplexes; ++i) {
      dst[2 * i]     = src[i].imag(); // Left = Q
      dst[2 * i + 1] = src[i].real(); // Right = I
    }
  } else {
    for (uint32_t i = 0; i < numComplexes; ++i) {
      dst[2 * i]     = src[i].real(); // Left = I
      dst[2 * i + 1] = src[i].imag(); // Right = Q
    }
  }
  return numComplexes * 2;
}