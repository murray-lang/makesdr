#pragma once
#include "Modulator.h"
#include <iq/oscillator/OscillatorInjector.h>

class CwModulator : public Modulator
{
public:
  CwModulator(const Mode::Proto& mode, uint32_t sampleRate) :
  Modulator(mode, sampleRate),
  m_oscillator(sampleRate, mode.offset)
  {
  };
  ~CwModulator() override = default;

  void setMode(const Mode::Proto& mode) override;

  uint32_t processSamples(const ComplexSamplesBuffer& audio, ComplexSamplesBuffer& output, uint32_t inputLength) override;

protected:
  Oscillator m_oscillator;
};
