#include "iq/rx/BasicIqRx.h"


BasicIqRx::BasicIqRx(const ModeList& modes)
  : IqRxBaseT()
  , m_pipelineA(modes)
{
}

ResultCode
BasicIqRx::configure(const Config::IqReceiver::Fields& iqReceiverConfig)
{
  ResultCode rc = m_iqIo.configure(iqReceiverConfig.iqIo);
  if (rc != ResultCode::OK) return rc;

  m_pipelineA.initialise(&m_iqIo, &m_iqIo);
  m_iqIo.setIqSink(this);
  return ResultCode::OK;
}

ResultCode
BasicIqRx::start()
{
  uint32_t framesPerOutputPacket = m_pipelineA.getMaxFramesPerOutputPacket();
  uint32_t framesPerInputPacket = m_pipelineA.getMaxFramesPerInputPacket();
  return m_iqIo.start(framesPerInputPacket, framesPerOutputPacket);
}

void
BasicIqRx::stop()
{
  m_iqIo.stop();
}

ResultCode
BasicIqRx::apply(BasicIqRxSettings& settings)
{
  if (settings.hasActiveBands()) {
    ActiveBandSettings* activeBandSettings = settings.activeBands();
    if (activeBandSettings != nullptr && activeBandSettings->hasFocusBand()) {
      BandSettings* bandSettings = activeBandSettings->focusBandSettings();
      if (bandSettings->hasFocusPipeline()) {
        RxPipelineSettings* pipelineSettings = bandSettings->focusPipeline();
        const BandRfSettings* bandRfSettings = bandSettings->hasRfSettings() ? bandSettings->rfSettings() : nullptr;

        return m_pipelineA.apply(bandRfSettings, pipelineSettings);
      }
    }
  }
  return ResultCode::OK;
}

// ResultCode
// BasicIqRx::apply(const BandSettings& bandSettings)
// {
//
// }

uint32_t
BasicIqRx::sinkIq(ComplexPingPongBuffers& samples, uint32_t length)
{
  // EventDispatcher::postEvent(m_eventTarget, new ReceiverIqEvent(samples, length, m_iqIo.getInputSampleRate() ));
  return m_pipelineA.sinkIq(samples, length);
}
