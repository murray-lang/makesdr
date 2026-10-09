#pragma once
#include <CrossPlatformTypes.h>
#include <iq/pipeline/stage/IqPipelineStage.h>
#include <iq/pipeline/stage/SampleRateProvider.h>
// #include <event/EventTarget.h>

#include <utility>

#include <transport/radio/IqPublisher.h>
#include <transport/radio/FftPublisher.h>

#include <settings/model/radio/PipelineId.h>
#include <dsp/transforms/power-spectrum/PowerSpectrumT.h>

class MonitorStage
  : public IqPipelineStage
{
public:
  MonitorStage(PipelineId pipelineId, IqPublisher* iqPublisher, FftPublisher* fftPublisher)
    : m_pipelineId(pipelineId)
    , m_iqPublisher(iqPublisher)
    , m_fftPublisher(fftPublisher)
    , m_sampleRateProvider([]() { return 0; })
    , m_enabled(false)
    , m_powerSpectrum()
    // , m_fft(WindowType::HANNING)
  {
  }

  void setSampleRateProvider(SampleRateProvider sampleRateProvider) {
    m_sampleRateProvider = ::move(sampleRateProvider);
  }

  void enable(bool enable) {
    m_enabled = enable;
  }

  // Number of input buffers averaged into each published spectrum (minimum 1)
  void setAverageBuffers(uint32_t buffers) {
    m_averageBuffers = buffers == 0 ? 1 : buffers;
  }
  [[nodiscard]] uint32_t averageBuffers() const { return m_averageBuffers; }

  uint32_t processSamples(ComplexPingPongBuffers& buffers, uint32_t inputLength) override;

protected:
  PipelineId m_pipelineId;
  IqPublisher* m_iqPublisher;
  FftPublisher* m_fftPublisher;
  SampleRateProvider m_sampleRateProvider;
  bool m_enabled;
  uint32_t m_averageBuffers = 1;
  uint32_t m_buffersAccumulated = 0;
  // Member, not a global: Fft copies the global window functions, which aren't
  // guaranteed to be initialised before other translation units' globals.
  PowerSpectrumT<uint8_t, MONITOR_FFT_SIZE> m_powerSpectrum;
  // ComplexMonitorFft m_spectrum;
  // RealMonitorFft m_power;
  // Fft<MONITOR_FFT_SIZE> m_fft;
};
