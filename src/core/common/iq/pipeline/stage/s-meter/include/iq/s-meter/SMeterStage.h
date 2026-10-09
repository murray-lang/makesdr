#pragma once
#include <CrossPlatformTypes.h>
#include <iq/pipeline/stage/IqPipelineStage.h>
#include <iq/pipeline/stage/SampleRateProvider.h>
#include <iq/pipeline/stage/AgcGainDbProvider.h>
#include <transport/radio/RxMeteringPublisher.h>

#include <cmath>
#include <chrono>
#include <functional>

// Pass-through stage that estimates RSSI from complex IQ and posts meter updates.
// Intended placement: after channel filter + decimator, before any AGC.
class SMeterStage : public IqPipelineStage
{
public:
  SMeterStage(PipelineId pipelineId, RxMeteringPublisher* meteringPublisher, float smoothingTauSeconds = 0.20f, float uiUpdateHz = 20.0f);

  void setSampleRateProvider(SampleRateProvider sampleRateProvider) {
    m_sampleRateProvider = ::move(sampleRateProvider);
  }

  void setAgcGainProvider(const AgcGainDbProvider& agcGainDbProvider) {
    m_agcGainDbProvider = ::move(m_agcGainDbProvider);
  }

  void enable(bool enable) {
    m_enabled = enable;
  }

  uint32_t processSamples(ComplexPingPongBuffers& buffers, uint32_t inputLength) override;

private:
  bool m_enabled;
  PipelineId m_pipelineId;
  RxMeteringPublisher* m_meteringPublisher;
  SampleRateProvider m_sampleRateProvider;
  AgcGainDbProvider m_agcGainDbProvider;

  float m_tau;

  bool m_haveAvg{false};
  double m_pAvg{0.0};

  std::chrono::steady_clock::time_point m_lastPost{};
  std::chrono::duration<double> m_minUpdatePeriod;
};