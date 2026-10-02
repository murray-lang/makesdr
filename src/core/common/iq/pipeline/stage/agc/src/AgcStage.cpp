#include "iq/agc/AgcStage.h"

AgcStage::AgcStage(AgcSpeed speed) :
    m_agc(nullptr),
    m_lastGain(1.0f),
    m_speed(speed)
{
  m_agc = agc_crcf_create();
  if (!m_agc) {
    // throw std::runtime_error("agc_crcf_create() failed");
  }
  agc_crcf_set_scale(m_agc, 0.25);
  setBandwidth(speed);
}

AgcStage::~AgcStage() {
  if (m_agc) {
    agc_crcf_destroy(m_agc);
    m_agc = nullptr;
  }
}

void
AgcStage::setSpeed(AgcSpeed speed)
{
  m_speed = speed;
  setBandwidth(speed);
}

uint32_t
AgcStage::processSamples(ComplexPingPongBuffers& buffers, uint32_t inputLength)
{
  if (m_speed == AgcSpeed::OFF) {
    buffers.flip();
    return inputLength;
  }
  agc_crcf_execute_block(m_agc, buffers.input().data(), inputLength, buffers.output().data() );

  m_lastGain.store(agc_crcf_get_gain(m_agc), std::memory_order_relaxed);
  return inputLength;
}

float
AgcStage::getGainDb() const
{
  const float g = std::max(m_lastGain.load(std::memory_order_relaxed), 1e-20f);
  return 20.0f * std::log10(g);
}

float
AgcStage::speedToBandwidth(AgcSpeed speed) const
{
  switch (speed) {
  case AgcSpeed::FAST:
    return FAST_LOOP_BANDWIDTH;
  case AgcSpeed::MEDIUM:
    return MEDIUM_LOOP_BANDWIDTH;
  case AgcSpeed::SLOW:
    return SLOW_LOOP_BANDWIDTH;
  default:
    return DEFAULT_LOOP_BANDWIDTH;
  }
}

void
AgcStage::setBandwidth(AgcSpeed speed)
{
  float bw = speedToBandwidth(speed);
  setBandwidth(bw);
}

void
AgcStage::setBandwidth(float bandwidth)
{
  agc_crcf_set_bandwidth(m_agc, bandwidth);
}