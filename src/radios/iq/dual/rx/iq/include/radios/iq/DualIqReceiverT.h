#pragma once

#include <iq/base/IqRxBaseT.h>
#include <iq/io/IqIo.h>
#include <iq/pipeline/IqRxPipeline.h>
#include <audio/mixer/AudioMixer.h>

#include <transport/IRadioPublishers.h>
#include <config/struct/SdrConfig.h>

template<typename RadioSettingsT>
class DualIqReceiverT : public IqRxBaseT<RadioSettingsT>
{
public:
  using RadioSettings = RadioSettingsT;
  using ActiveBandSettings = typename RadioSettingsT::ActiveBandSettings;
  using BandSettings = typename RadioSettingsT::BandSettings;

  DualIqReceiverT(
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
  ~DualIqReceiverT() override
  {
    DualIqReceiverT::stop(); // Joins the IQ source thread. Idempotent if already stopped.
  }

  // ResultCode configure(const Config::Sdr::Fields& sdrConfig)
  // {
  //   if (sdrConfig.receiver) {
  //     const Config::IqReceiver::Fields& rxConfig = sdrConfig.receiver.value();
  //     ResultCode rc = m_iqIo.configure(rxConfig.iqIo);
  //     if (rc != ResultCode::OK) return rc;
  //
  //     m_rxPipelineA.initialise(&m_iqIo, &m_mixer.inputA());
  //     m_rxPipelineB.initialise(&m_iqIo, &m_mixer.inputB());
  //     m_iqIo.setIqSink(this);
  //     return ResultCode::OK;
  //   }
  //   return ResultCode::ERR_CONFIG_RXTX_NO_RX;
  // }
  ResultCode configure(const Config::IqReceiver::Fields& iqReceiverConfig) override
  {
    ResultCode rc = m_iqIo.configure(iqReceiverConfig.iqIo);
    if (rc != ResultCode::OK) return rc;

    m_rxPipelineA.initialise(&m_iqIo, &m_mixer.inputA());
    m_rxPipelineB.initialise(&m_iqIo, &m_mixer.inputB());
    m_iqIo.setIqSink(this);
    return ResultCode::OK;
  }


  ResultCode start()
  {
    uint32_t framesPerOutputPacket = m_rxPipelineA.getMaxFramesPerOutputPacket();
    uint32_t framesPerInputPacket = m_rxPipelineA.getMaxFramesPerInputPacket();
    return m_iqIo.start(framesPerInputPacket, framesPerOutputPacket);
  }

  void stop()
  {
    m_iqIo.stop();
  }

  ResultCode apply(RadioSettings& settings) override
  {
    ResultCode rc = ResultCode::OK;
    if (settings.hasActiveBands()) {
      ActiveBandSettings* activeBandSettings = settings.activeBands();

      if (activeBandSettings != nullptr && activeBandSettings->hasFocusBand()) {
        BandSettings* bandSettings = activeBandSettings->focusBandSettings();
        rc = apply(bandSettings);
        if (rc != ResultCode::OK) return rc;
      }
    }
    return rc;
  }


  ResultCode apply(IBandSettings* bandSettings)
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

  uint32_t sinkIq(ComplexSamplesBuffer& samples, uint32_t length) override
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

  ResultCode ptt(bool on) override
  {
    return ResultCode::OK;
  }

  IqRxPipeline* focusPipeline(IBandSettings* bandSettings)
  {
    switch (bandSettings->focusPipelineId()) {
    case PipelineId::A: return &m_rxPipelineA;
    case PipelineId::B: return &m_rxPipelineB;
    default: return nullptr;
    }
  }

protected:
  IqIo m_iqIo;
  ComplexPingPongBuffers& m_pingPongBuffers;
  IqRxPipeline m_rxPipelineA;
  IqRxPipeline m_rxPipelineB;
  bool m_pipelineBEnabled;
  AudioMixer m_mixer;
};
