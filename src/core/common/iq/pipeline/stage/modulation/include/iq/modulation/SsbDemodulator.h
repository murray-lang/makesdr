#pragma once

#include "Demodulator.h"

class SsbDemodulator : public Demodulator
{
public:
  SsbDemodulator(const Mode::Proto& mode, uint32_t sampleRate) :
    Demodulator(mode, sampleRate)
  {}

  uint32_t processSamples(
      const ComplexSamplesBuffer& in,
      RealSamplesRef& out,
      uint32_t inputLength
  ) override;

};
