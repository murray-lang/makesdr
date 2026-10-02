#include "iq/modulation/FmModulator.h"

FmModulator::FmModulator(const Mode::Proto& mode, uint32_t sampleRate)
  : Modulator(mode, sampleRate)
  , m_mod(nullptr)
{
  FmModulator::setMode(mode);
}
FmModulator::~FmModulator()
{
  clearState();
}

void
FmModulator::setMode(const Mode::Proto& mode)
{
  clearState();
  Modulator::setMode(mode);
  float modulationFactor =
    (static_cast<float>(mode.hi_cut) - static_cast<float>(mode.lo_cut))
    / static_cast<float>(m_sampleRate);
  m_mod = freqmod_create(modulationFactor);
}

uint32_t
FmModulator::processSamples(
    const ComplexSamplesBuffer& in,
    ComplexSamplesBuffer& out,
    uint32_t inputLength
)
{
  for (uint32_t i = 0; i < inputLength; i++) {
    liquid_float_complex sampleOut;
    // freqmod only needs a real value, so we take the hypotenuse of the analytic signal
    freqmod_modulate(m_mod, std::abs(in.at(i)), &sampleOut);
    out.at(i) = sdrcomplex(sampleOut.real, sampleOut.imag);
  }
  return inputLength;
}

void
FmModulator::clearState()
{
  if (m_mod) {
    freqmod_destroy(m_mod);
    m_mod = nullptr;
  }
}