#include "iq/rxtx/DualIqRxTx_Rx.h"

DualIqRxTx_Rx::DualIqRxTx_Rx(const ModeList& modes)
  : m_rxPipelineA(modes)
  , m_rxPipelineB(modes)
  , m_pipelineBEnabled(false)
  , m_mixer(m_iqIo)
{
}

ResultCode
DualIqRxTx_Rx::configure(const Config::Sdr::Fields& sdrConfig)
{
  if (sdrConfig.receiver) {
    const Config::IqReceiver::Fields& rxConfig = sdrConfig.receiver.value();
    ResultCode rc = m_iqIo.configure(rxConfig.iqIo);
    if (rc != ResultCode::OK) return rc;

    m_rxPipelineA.initialise(&m_iqIo, &m_mixer.inputA());
    m_rxPipelineB.initialise(&m_iqIo, &m_mixer.inputB());
    m_iqIo.setIqSink(this);
    return ResultCode::OK;
  }
  return ResultCode::ERR_CONFIG_RXTX_NO_RX;
}

ResultCode
DualIqRxTx_Rx::start()
{
  uint32_t framesPerOutputPacket = m_rxPipelineA.getMaxFramesPerOutputPacket();
  uint32_t framesPerInputPacket = m_rxPipelineA.getMaxFramesPerInputPacket();
  return m_iqIo.start(framesPerInputPacket, framesPerOutputPacket);
}

void
DualIqRxTx_Rx::stop()
{
  m_iqIo.stop();
}

ResultCode
DualIqRxTx_Rx::apply(IBandSettings* bandSettings)
{
  bool dualPipelineChanged = false;
  if (bandSettings->hasIsMultiPipeline()) {
    bool isDualPipeline = bandSettings->isMultiPipeline();
    if (isDualPipeline != m_pipelineBEnabled) {
      dualPipelineChanged = true;
      m_pipelineBEnabled = isDualPipeline;
      m_mixer.setInputBEnabled(m_pipelineBEnabled);
    }
  }
  BandRfSettings* bandRfSettings = bandSettings->hasRfSettings() ? bandSettings->rfSettings() : nullptr;
  if (dualPipelineChanged) {
    if (bandSettings->hasPipeline(PipelineId::A)) {
      RxPipelineSettings* rxPipelineASettings = bandSettings->pipeline(PipelineId::A);
      m_rxPipelineA.apply(bandRfSettings, rxPipelineASettings);
    }
    if (m_pipelineBEnabled) {
      if (bandSettings->hasPipeline(PipelineId::B)) {
        RxPipelineSettings* rxPipelineBSettings = bandSettings->pipeline(PipelineId::B);
        m_rxPipelineB.apply(bandRfSettings, rxPipelineBSettings);
      }
    }
  } else if (bandSettings->hasFocusPipeline()) {
    RxPipelineSettings* focusPipelineSettings = bandSettings->focusPipeline();
    if (focusPipelineSettings != nullptr) {
      IqRxPipeline* focusRxPipeline = focusPipeline(bandSettings);
      if (focusRxPipeline != nullptr) {
        focusRxPipeline->apply(bandRfSettings, focusPipelineSettings);
      }
    }
  }
  if (bandSettings->hasFocusPipelineId()) {
    // TODO: Handle any monitoring changes etc.
  }
  return ResultCode::OK;
}

uint32_t
DualIqRxTx_Rx::sinkIq(ComplexPingPongBuffers& samples, uint32_t length)
{
  uint32_t outA = m_rxPipelineA.sinkIq(samples, length);

  // Feed pipeline B only when enabled
  if (m_pipelineBEnabled) {
    m_rxPipelineB.sinkIq(samples, length);
  }

  return outA;
}

IqRxPipeline*
DualIqRxTx_Rx::focusPipeline(IBandSettings* bandSettings)
{
  switch (bandSettings->focusPipelineId()) {
    case PipelineId::A: return &m_rxPipelineA;
    case PipelineId::B: return &m_rxPipelineB;
    default: return nullptr;
  }
}
