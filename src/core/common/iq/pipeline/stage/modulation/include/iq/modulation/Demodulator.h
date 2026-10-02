#pragma once

#include <samples/SampleTypes.h>
#include <settings/model/radio/Mode.h>

class Demodulator
{
public:
  Demodulator(const Mode::Proto& mode, uint32_t sampleRate) :
    m_modeType(static_cast<Mode::Type>(mode.type)),
    m_sampleRate(sampleRate)
  {
  }

  virtual ~Demodulator() = default;

  virtual uint32_t processSamples(
    const ComplexSamplesBuffer& in,
    RealSamplesBuffer& out,
    uint32_t inputLength
    ) = 0;

  const Mode::Type& getModeType() { return m_modeType; }
  virtual void setMode(const Mode::Proto& mode) { m_modeType = static_cast<Mode::Type>(mode.type); }

  virtual void setSampleRate(uint32_t sampleRate) {
    m_sampleRate = sampleRate;
  }
  virtual uint32_t getSampleRate() {
    return m_sampleRate;
  }

  [[nodiscard]] virtual uint32_t getNumOutputChannels() const { return 1; }

protected:
  Mode::Type m_modeType;
  uint32_t m_sampleRate;

};
