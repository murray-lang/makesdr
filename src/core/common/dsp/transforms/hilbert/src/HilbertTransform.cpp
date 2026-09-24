#include "dsp/transforms/hilbert/HilbertTransform.h"

HilbertTransform::HilbertTransform(int numTaps, sdrreal upperBandFraction)
    : m_hilbertState(nullptr)
{
  initialise(numTaps, upperBandFraction);
}

HilbertTransform::~HilbertTransform()
{
  clearState();
}

void
HilbertTransform::initialise(int numTaps, sdrreal stopBandAttenuation)
{
  clearState();
  // Liquid's firhilbf_create(m, as)
  // m is the filter semi-length (total taps ~ 4m)
  // as is the stop-band attenuation in dB
  unsigned int m = static_cast<unsigned int>(numTaps) / 4;
  if (m < 1) {
    m = 1;
  }
  m_hilbertState = firhilbf_create(m, stopBandAttenuation);
}


uint32_t
HilbertTransform::transform(const RealSamplesBuffer& input, uint32_t numSamples, uint32_t numChannels, ComplexSamplesBuffer& output)
{
  // qDebug() << "HilbertTransform::transform(): numSamples =" << numSamples << ", numChannels =" << numChannels;
  auto N = static_cast<size_t>(numSamples/numChannels);

  for (size_t n = 0; n < N; ++n) {
    size_t i = n * numChannels;

    std::complex<float> y;
    firhilbf_r2c_execute(m_hilbertState, static_cast<float>(input.at(i)), &y);

    output.at(i) = y;
  }
  return N;
}

void
HilbertTransform::clearState()
{
  if (m_hilbertState) {
    firhilbf_destroy(m_hilbertState);
    m_hilbertState = nullptr;
  }
}