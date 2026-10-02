#include "iq/modulation/CwDemodulator.h"

uint32_t
CwDemodulator::processSamples(
    const ComplexSamplesBuffer& in,
    RealSamplesBuffer& out,
    uint32_t inputLength)
{
  if (inputLength == 0) {
    return 0;
  }
  out.resize(inputLength);

  if (m_modeType == Mode::Type::CWU) {
    for (uint32_t i = 0; i < inputLength; ++i) {
      out.at(i) = in.at(i).real() * 10.0f;
    }
  } else {
    for (uint32_t i = 0; i < inputLength; ++i) {
      out[i] = -in[i].real() * 10.0f;
    }
  }


  // (OPTIONAL) Remove DC offset, if desired
  sdrreal sum = 0.0;
  for (uint32_t i = 0; i < inputLength; ++i) sum += out[i];
  sdrreal avg = sum / static_cast<sdrreal>(inputLength);
  for (uint32_t i = 0; i < inputLength; ++i) out[i] -= avg;

  return inputLength;
}