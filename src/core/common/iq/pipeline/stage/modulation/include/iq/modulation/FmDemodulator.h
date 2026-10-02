#pragma once

#include "Demodulator.h"
#include <liquid/liquid.h>

class FmDemodulator : public Demodulator
{
public:
  explicit FmDemodulator(const Mode::Proto& mode, uint32_t sampleRate);
  ~FmDemodulator() override;

  void setMode(const Mode::Proto& mode) override;

  uint32_t processSamples(
      const ComplexSamplesBuffer& in,
      RealSamplesBuffer& out,
      uint32_t inputLength
  ) override;

  // uint32_t processSamples(PingPongBuffers<sdrcomplex> buffers, uint32_t inputLength) override;
protected:
  void clearState();

protected:
  sdrcomplex m_prevSample;
  freqdem m_demod;
};
