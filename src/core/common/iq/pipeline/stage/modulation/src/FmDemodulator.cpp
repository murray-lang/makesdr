#include "iq/modulation/FmDemodulator.h"

FmDemodulator::FmDemodulator(const Mode::Proto& mode, uint32_t sampleRate)
  : Demodulator(mode, sampleRate)
  , m_prevSample(static_cast<sdrreal>(1.0), static_cast<sdrreal>(0.0))
  , m_demod(nullptr)
{
  FmDemodulator::setMode(mode);
}

FmDemodulator::~FmDemodulator()
{
  clearState();
}

void
FmDemodulator::setMode(const Mode::Proto& mode)
{
  clearState();
  Demodulator::setMode(mode);
  float modulationFactor =
    (static_cast<float>(mode.hi_cut) - static_cast<float>(mode.lo_cut))
    / static_cast<float>(m_sampleRate);
  m_demod = freqdem_create(modulationFactor);
}


uint32_t
FmDemodulator::processSamples(
    const ComplexSamplesBuffer& in,
    RealSamplesBuffer& out,
    uint32_t inputLength)
{
  for(uint32_t i=0; i<inputLength; i++) {
    sdrcomplex sample = in.at(i);
    float outSample;
    freqdem_demodulate(m_demod, sample, &outSample);
    out.at(i) = outSample;
  }
  return inputLength;
}

void
FmDemodulator::clearState()
{
  if (m_demod) {
    freqdem_destroy(m_demod);
    m_demod = nullptr;
  }
}