#pragma once

#include "Demodulator.h"
#include <liquid/liquid.h>


class AmDemodulator : public Demodulator
{
public:
  AmDemodulator(const Mode::Proto& mode, uint32_t sampleRate)
   : Demodulator(mode, sampleRate)
    , m_zero(0.0f)
    , m_demodulator(nullptr)
  {
    m_demodulator = ampmodem_create(0.5f, LIQUID_AMPMODEM_DSB, 0);
  }
  ~AmDemodulator() override
  {
    ampmodem_destroy(m_demodulator);
  }

  uint32_t processSamples(
      const ComplexSamplesBuffer& in,
      RealSamplesBuffer& out,
      uint32_t inputLength
  ) override;

protected:
  sdrreal m_zero;
  ampmodem m_demodulator;
};
