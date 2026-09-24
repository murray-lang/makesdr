#pragma once

#include "Demodulator.h"


class AmDemodulator : public Demodulator
{
public:
  AmDemodulator(const Mode::Proto& mode, uint32_t sampleRate) :
    Demodulator(mode, sampleRate),
    m_zero(0.0f)
    {}

  uint32_t processSamples(
      const ComplexSamplesBuffer& in,
      RealSamplesBuffer& out,
      uint32_t inputLength
  ) override;

protected:
  sdrreal m_zero;

};
