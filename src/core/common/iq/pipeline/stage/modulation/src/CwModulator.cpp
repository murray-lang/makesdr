#include "iq/modulation/CwModulator.h"


void
CwModulator::setMode(const Mode::Proto& mode)
{
  Modulator::setMode(mode);
  m_oscillator.setFrequency(mode.offset);
}

uint32_t
CwModulator::processSamples(const ComplexSamplesBuffer& audio, ComplexSamplesBuffer& output, uint32_t inputLength)
{
  // The input signal provides nothing more than a clock for the CW oscillator
  for (uint32_t i = 0; i < inputLength; i++) {
    output[i] = m_oscillator.getState() * 1.0f; //m_inputGain;
    ++m_oscillator;
  }
  return inputLength;
}