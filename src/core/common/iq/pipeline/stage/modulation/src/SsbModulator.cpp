#include "iq/modulation/SsbModulator.h"

SsbModulator::SsbModulator(const Mode::Proto& mode, uint32_t sampleRate) :
  Modulator(mode, sampleRate)
{}

uint32_t
SsbModulator::processSamples(const ComplexSamplesBuffer& audio, ComplexSamplesBuffer& output, uint32_t inputLength)
{

  if (m_modeType == Mode::LSB) {
    for (uint32_t i = 0; i < inputLength; ++i) {
      output.at(i) = sdrcomplex(audio[i].real(), -audio[i].imag()) * m_inputGain;
    }
  } else {
    for (uint32_t i = 0; i < inputLength; ++i) {
      output.at(i) = audio.at(i) * m_inputGain;
    }
  }
  return inputLength;
}