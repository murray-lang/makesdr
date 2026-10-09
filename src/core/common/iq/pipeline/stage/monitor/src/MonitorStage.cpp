#include "iq/monitor/MonitorStage.h"
#include <samples/SampleTypes.h>
#include <limits>

uint32_t
MonitorStage::processSamples(ComplexPingPongBuffers& buffers, uint32_t inputLength)
{
  // if (m_iqPublisher != nullptr && m_enabled) {
  //   if (IqMessage* msg = m_iqPublisher->reserve()) {
  //     msg->initialise(m_pipelineId, buffers.input(), inputLength, m_sampleRateProvider());
  //     m_iqPublisher->commit(msg);
  //   }
  // }
  if (m_fftPublisher == nullptr || !m_enabled) {
    // Don't carry stale power over to when monitoring resumes
    m_powerSpectrum.reset();
    m_buffersAccumulated = 0;
    buffers.flip();
    return inputLength;
  }

  if (m_powerSpectrum.accumulate(buffers.input().data(), inputLength) > 0) {
    ++m_buffersAccumulated;
  }

  if (m_buffersAccumulated >= m_averageBuffers) {
    // If no message slot is free, drop this average rather than letting it grow stale
    if (FftMessage* msg = m_fftPublisher->reserve()) {
      msg->initialise(m_pipelineId, m_sampleRateProvider(), m_powerSpectrum.floor(), m_powerSpectrum.ceiling());
      makesdr_FftPb_bins_t& bins = msg->bins();
      m_powerSpectrum.render(bins.bytes);
      bins.size = MONITOR_FFT_SIZE;
      m_fftPublisher->commit(msg);
    }
    m_powerSpectrum.reset();
    m_buffersAccumulated = 0;
  }

  buffers.flip();
  return inputLength;
}
