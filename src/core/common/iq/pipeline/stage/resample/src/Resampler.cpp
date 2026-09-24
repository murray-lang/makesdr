#include "iq/resample/Resampler.h"

#include <cmath>

Resampler::Resampler() :
  m_inputSampleRate(0),
  m_outputSampleRate(0),
  m_pResampleStateReal(nullptr),
  m_pResampleStateComplex(nullptr),
  m_ratio(0.0)
{
}

Resampler::~Resampler()
{
  clearState();
}

uint32_t
Resampler::configure(uint32_t inputSampleRate, uint32_t outputSampleRate)
{
  m_inputSampleRate = inputSampleRate;
  m_outputSampleRate = outputSampleRate;
  initialise();
  return m_outputSampleRate;
}

void
Resampler::initialise()
{
  clearState();
  float ratio = static_cast<float>(m_outputSampleRate) / static_cast<float>(m_inputSampleRate);

  // Liquid-dsp resampler parameters
  unsigned int m = 12; // filter semi-length (delay)
  float bw = 0.45f; // resampling bandwidth
  float as = 100.0f; // stop-band attenuation [dB]

  m_pResampleStateReal = resamp_rrrf_create(ratio, m, bw, as, 32);
  m_pResampleStateComplex = resamp_crcf_create(ratio, m, bw, as, 32);

  m_ratio = ratio;
}

uint32_t
Resampler::processSamples(
  const RealSamplesBuffer& in,
  RealSamplesBuffer& out,
  uint32_t inputLength
) const
{
  unsigned int numWritten = 0;
  for (uint32_t i = 0; i < inputLength; ++i) {
    unsigned int n;
    resamp_rrrf_execute(m_pResampleStateReal, in.at(i), &out.at(numWritten), &n);
    numWritten += n;
  }
  return numWritten;
}

uint32_t
Resampler::processSamples(
  ComplexPingPongBuffers& buffers,
  uint32_t inputLength
)
{
  ComplexSamplesBuffer& in = buffers.input();
  ComplexSamplesBuffer& out = buffers.output();
  // resamp_crcf_execute writes a burst of up to ceil(ratio)+1 samples per input,
  // so the write cursor has to leave room for a whole burst. At ratio 1.0 the
  // output is as long as the input, which fills out completely.
  const unsigned int maxBurst = static_cast<unsigned int>(std::ceil(m_ratio)) + 1;
  const unsigned int capacity = out.size();
  unsigned int numWritten = 0;
  for (uint32_t i = 0; i < inputLength; ++i) {
    if (numWritten + maxBurst > capacity) {
      break; // Buffer too small for this ratio - see PIPELINE_BUFFER_LENGTH.
    }
    unsigned int n;
    // liquid-dsp uses float complex which is usually binary compatible with std::complex<float>
    resamp_crcf_execute(m_pResampleStateComplex, in.at(i), &out.at(numWritten), &n);
    numWritten += n;
  }

  return numWritten;
}

void
Resampler::clearState()
{
  if (m_pResampleStateReal) {
    resamp_rrrf_destroy(m_pResampleStateReal);
    m_pResampleStateReal = nullptr;
  }
  if (m_pResampleStateComplex) {
    resamp_crcf_destroy(m_pResampleStateComplex);
    m_pResampleStateComplex = nullptr;
  }
}
