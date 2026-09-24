#pragma once
#include <liquid/liquid.h>

#include "Modulator.h"

class FmModulator : public Modulator
{
public:
  explicit FmModulator(const Mode::Proto& mode, uint32_t sampleRate);
  ~FmModulator() override;

  void setMode(const Mode::Proto& mode) override;

  uint32_t processSamples(
      const ComplexSamplesBuffer& in,
      ComplexSamplesBuffer& out,
      uint32_t inputLength
  ) override;

protected:
  void clearState();

protected:
  freqmod m_mod;
};