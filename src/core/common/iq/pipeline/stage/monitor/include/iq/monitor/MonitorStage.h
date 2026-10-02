#pragma once
#include <CrossPlatformTypes.h>
#include <iq/pipeline/stage/IqPipelineStage.h>
#include <event/EventTarget.h>

#include <utility>

#include <transport/radio/IqPublisher.h>

using SampleRateProvider = function<uint32_t()>;

class MonitorStage
  : public IqPipelineStage
{
public:
  MonitorStage(IqPublisher* iqPublisher)
    : m_iqPublisher(iqPublisher)
    , m_sampleRateProvider([]() { return 0; })
    , m_enabled(false)
  {
  }

  void setSampleRateProvider(SampleRateProvider sampleRateProvider) {
    m_sampleRateProvider = ::move(sampleRateProvider);
  }

  void enable(bool enable) {
    m_enabled = enable;
  }

  uint32_t processSamples(ComplexPingPongBuffers& buffers, uint32_t inputLength) override
  {
    if (m_iqPublisher != nullptr && m_enabled) {
      if (IqMessage* msg = m_iqPublisher->reserve()) {
        msg->initialise(buffers.input(), inputLength, m_sampleRateProvider());
        m_iqPublisher->commit(msg);
      }
    }
    buffers.flip();
    return inputLength;
  }

protected:
  IqPublisher* m_iqPublisher;
  SampleRateProvider m_sampleRateProvider;
  bool m_enabled;
};