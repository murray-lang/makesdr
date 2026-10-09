#include "iq/common/IqTransmitter.h"


IqTransmitter::IqTransmitter(ComplexPingPongBuffers& pingPongBuffers, const ModeList& modes)
  : m_pingPongBuffers(pingPongBuffers)
  , m_txPipeline(PipelineId::NONE, modes)
{
}

ResultCode
IqTransmitter::configure(const Config::Sdr::Fields& sdrConfig)
{
  if (sdrConfig.transmitter) {
    const Config::IqTransmitter::Fields txConfig = sdrConfig.transmitter.value();
    ResultCode rc = m_iqIo.configure(txConfig.iqIo);
    if (rc != ResultCode::OK) return rc;
    // The AudioSink provided here could be m_pPipelineIo, but...
    // this class intercepts the audio out for display purposes.
    m_txPipeline.initialise(&m_iqIo, dynamic_cast<AudioSink*>(this));
    // This class also intercepts the received IQ for display purposes.
    m_iqIo.setIqSink(this);
    return ResultCode::OK;
  }
  return ResultCode::ERR_CONFIG_RXTX_NO_TX;
}

ResultCode
IqTransmitter::start()
{
  uint32_t framesPerOutputPacket = m_txPipeline.getMaxFramesPerOutputPacket();
  uint32_t framesPerInputPacket = m_txPipeline.getMaxFramesPerInputPacket();
  return m_iqIo.start(framesPerInputPacket, framesPerOutputPacket);
}

void
IqTransmitter::stop()
{
  m_iqIo.stop();
}

ResultCode
IqTransmitter::apply(const BandRfSettings* bandRfSettings, PipelineSettings* settings)
{
  return m_txPipeline.apply(bandRfSettings, settings);
}

uint32_t
IqTransmitter::sinkIq(ComplexSamplesBuffer& samples, uint32_t length)
{
  m_pingPongBuffers.reset();
  std::copy_n(samples.begin(), length, m_pingPongBuffers.input().begin());
  return m_txPipeline.processSamples(m_pingPongBuffers, length);
}

uint32_t
IqTransmitter::sinkAudio(const RealSamplesBuffer& samples, uint32_t length, uint32_t numChannels)
{
  return m_iqIo.sinkAudio(samples, length, numChannels);
}