#include "iq/s-meter/SMeterStage.h"

SMeterStage::SMeterStage(
  PipelineId pipelineId,
  RxMeteringPublisher* meteringPublisher,
  float smoothingTauSeconds,
  float uiUpdateHz
  )
  : m_enabled(false)
  , m_pipelineId(pipelineId)
  , m_meteringPublisher(meteringPublisher)
  , m_sampleRateProvider([]() { return 0; })
  , m_agcGainDbProvider([]() { return 0.0; })
  , m_tau(std::max(0.01f, smoothingTauSeconds))
  , m_minUpdatePeriod(std::chrono::duration<double>(1.0 / std::max(1.0f, uiUpdateHz)))
  {
}

uint32_t
SMeterStage::processSamples(ComplexPingPongBuffers& buffers, uint32_t inputLength)
{
  if (m_meteringPublisher == nullptr || inputLength == 0 || !m_enabled) {
    buffers.flip();
    return inputLength;
  }
  uint32_t sampleRate = m_sampleRateProvider();
  if (sampleRate == 0) {
    buffers.flip();
    return inputLength;
  }

  RxMeteringMessage* metering = m_meteringPublisher->reserve();
  if (metering == nullptr) {
    buffers.flip();
    return inputLength;
  }
  const ComplexSamplesBuffer& in = buffers.input();

  // Instantaneous mean power over the block: mean(|x|^2)
  double acc = 0.0;
  for (uint32_t i = 0; i < inputLength; ++i) {
    const auto& x = in[i];
    const auto re = static_cast<double>(x.real());
    const auto im = static_cast<double>(x.imag());
    acc += re * re + im * im;
  }
  const double pInst = acc / static_cast<double>(inputLength);

  // Exponential smoothing with time-constant tau
  const double dt = static_cast<double>(inputLength) / static_cast<double>(sampleRate);
  const double a = 1.0 - std::exp(-dt / static_cast<double>(m_tau));

  if (!m_haveAvg) {
    m_pAvg = pInst;
    m_haveAvg = true;
  } else {
    m_pAvg = (1.0 - a) * m_pAvg + a * pInst;
  }

  // Rate-limit UI events
  const auto now = std::chrono::steady_clock::now();
  if ((now - m_lastPost) >= m_minUpdatePeriod) {
    m_lastPost = now;

    constexpr double eps = 1e-20;
    const double p = std::max(m_pAvg, eps);
    auto rssiDbFs = static_cast<float>(10.0 * std::log10(p));

    std::optional<float> agcGainDb;
    float agcGain = m_agcGainDbProvider();

    metering->initialise(m_pipelineId, rssiDbFs, agcGain);
    m_meteringPublisher->commit(metering);
  }
  buffers.flip();
  return inputLength;
}