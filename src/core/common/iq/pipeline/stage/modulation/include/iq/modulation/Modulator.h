#pragma once

#include <samples/SampleTypes.h>
#include <settings/model/radios/Mode.h>

class Modulator
{
public:
  Modulator(const Mode::Proto& mode, uint32_t sampleRate)
    : m_modeType(static_cast<Mode::Type>(mode.type))
    , m_sampleRate(sampleRate)
    , m_inputGain(1.0) {}
  virtual ~Modulator() = default;

  virtual uint32_t processSamples(const ComplexSamplesMax& audio, ComplexSamplesMax& output, uint32_t inputLength) = 0;

  virtual void setSampleRate(uint32_t sampleRate) {
    m_sampleRate = sampleRate;
  }
  virtual uint32_t getSampleRate() {
    return m_sampleRate;
  }

  const Mode::Type& getModeType() { return m_modeType; }
  virtual void setMode(const Mode::Proto& mode) { m_modeType = static_cast<Mode::Type>(mode.type); }

  void setInputGain(sdrreal gain) { m_inputGain = gain; }
  [[nodiscard]] sdrreal getInputGain() const { return m_inputGain; }

protected:
  Mode::Type m_modeType;
  uint32_t m_sampleRate;
  sdrreal m_inputGain;
};
