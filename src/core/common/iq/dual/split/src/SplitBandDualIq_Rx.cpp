#include "iq/split/SplitBandDualIq_Rx.h"


SplitBandDualIq_Rx::SplitBandDualIq_Rx(
  ComplexPingPongBuffers& pingPongBuffers,
  const BandCategoryList& bands,
  const ModeList& modes,
  IRadioPublishers* radioPublishers
  )
  : m_pingPongBuffers(pingPongBuffers)
  , m_rxPipelineA(PipelineId::A, modes, radioPublishers)
  , m_rxPipelineB(PipelineId::B, modes, radioPublishers)
  , m_pipelineBEnabled(false)
  , m_mixer(m_iqIo)
{
  m_rxPipelineA.enableMonitoring(true);
  m_rxPipelineA.enableMetering(true);
  m_rxPipelineB.enableMonitoring(false);
  m_rxPipelineB.enableMetering(false);

}

SplitBandDualIq_Rx::~SplitBandDualIq_Rx()
{
  stop(); // Joins the IQ source thread. Idempotent if already stopped.
}

ResultCode
SplitBandDualIq_Rx::configure(const Config::Sdr::Fields& sdrConfig)
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
SplitBandDualIq_Rx::start()
{
  uint32_t framesPerOutputPacket = m_rxPipelineA.getMaxFramesPerOutputPacket();
  uint32_t framesPerInputPacket = m_rxPipelineA.getMaxFramesPerInputPacket();
  return m_iqIo.start(framesPerInputPacket, framesPerOutputPacket);
}

void
SplitBandDualIq_Rx::stop()
{
  m_iqIo.stop();
}

ResultCode
SplitBandDualIq_Rx::apply(IBandSettings* bandSettings)
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
  const BandRfSettings* bandRfSettings = bandSettings->rfSettings(); // Always get these for RF offset calcs.
  // if (dualPipelineChanged) {
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
  // }
  // else if (bandSettings->hasFocusPipeline()) {
  //   RxPipelineSettings* focusPipelineSettings = bandSettings->focusPipeline();
  //   if (focusPipelineSettings != nullptr) {
  //     IqRxPipeline* focusRxPipeline = focusPipeline(bandSettings);
  //     if (focusRxPipeline != nullptr) {
  //       focusRxPipeline->apply(bandRfSettings, focusPipelineSettings);
  //     }
  //   }
  // }
  if (bandSettings->hasFocusPipelineId()) {
    // TODO: Handle any monitoring changes etc.
  }
  return ResultCode::OK;
}

uint32_t
SplitBandDualIq_Rx::sinkIq(ComplexSamplesBuffer& samples, uint32_t length)
{
  m_pingPongBuffers.reset();
  std::copy_n(samples.begin(), length, m_pingPongBuffers.input().begin());
  uint32_t out = m_rxPipelineA.processSamples(m_pingPongBuffers, length);

  if (m_pipelineBEnabled) {
    m_pingPongBuffers.reset();
    std::copy_n(samples.begin(), length, m_pingPongBuffers.input().begin());
    out = m_rxPipelineB.processSamples(m_pingPongBuffers, length);
  }
  return out;
}

IqRxPipeline*
SplitBandDualIq_Rx::focusPipeline(IBandSettings* bandSettings)
{
  switch (bandSettings->focusPipelineId()) {
  case PipelineId::A: return &m_rxPipelineA;
  case PipelineId::B: return &m_rxPipelineB;
  default: return nullptr;
  }
}