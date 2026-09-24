#pragma once
#include "Demodulator.h"

class CwDemodulator : public Demodulator
{
public:  
  CwDemodulator(const Mode::Proto& mode, uint32_t sampleRate) :
      Demodulator(mode, sampleRate)
  {}

  uint32_t processSamples(
      const ComplexSamplesBuffer& in,
      RealSamplesBuffer& out,
      uint32_t inputLength
  ) override;
};
