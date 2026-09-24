#include "iq/correction/IqCorrection.h"

IqCorrection::IqCorrection() :
  m_amplitudeCorrection(0.0),
  m_phaseCorrection(0.0)
{
}

void IqCorrection::apply(const IqCorrectionSettings& correctionSettings)
{
  if (correctionSettings.hasAmplitude()) {
    m_amplitudeCorrection = correctionSettings.amplitude();
  }
  if (correctionSettings.hasPhase()) {
    m_phaseCorrection = correctionSettings.phase();
  }
}

uint32_t
IqCorrection::processSamples(ComplexPingPongBuffers& buffers, uint32_t inputLength)
{
  auto& input = buffers.input();
  auto& output = buffers.output();
  return processSamples(input, output, inputLength);
}

uint32_t
IqCorrection::processSamples(const ComplexSamplesBuffer& input, ComplexSamplesBuffer& output,
                                      uint32_t inputLength) const
{
  const sdrreal gain = static_cast<sdrreal>(1.0) + m_amplitudeCorrection;

  // Compute phase components.
  const std::complex<sdrreal> phasePhasor = std::polar(static_cast<sdrreal>(1.0), m_phaseCorrection);
  const sdrreal cosPhi = phasePhasor.real();
  const sdrreal sinPhi = phasePhasor.imag();

  for (uint32_t i = 0; i < inputLength; ++i) {
    const sdrcomplex& sample = input.at(i);

    // Apply IQ Imbalance Correction
    // 1. Remove the part of I that "leaked" into Q due to non-orthogonality (Phase Correction).
    //    Formula: Q_new = (Q_old - I * sin(phi)) / cos(phi)
    // 2. Normalize the gain of Q to match I (Amplitude Correction).

    sdrreal iVal = sample.real();
    sdrreal qVal = sample.imag();

    // Protect against divide by zero if phase is exactly 90 degrees (unlikely in practice)
    sdrreal qCorrected = (std::abs(cosPhi) > 1e-6) ? (qVal - iVal * sinPhi) / cosPhi : qVal;

    // Apply amplitude balance to Q
    qCorrected *= gain;

    output.at(i) = {iVal, qCorrected};
  }
  return inputLength;
}
