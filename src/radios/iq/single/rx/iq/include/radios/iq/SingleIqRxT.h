#pragma once

#include <iq/base/IqRxBaseT.h>
#include <iq/io/IqIo.h>
#include <iq/pipeline/IqRxPipeline.h>
#include <audio/mixer/AudioMixer.h>

#include <transport/IRadioPublishers.h>
#include <config/struct/SdrConfig.h>

template<typename RadioSettingsT>
class SingleIqRxT : public IqRxBaseT<RadioSettingsT>
{
public:
  using RadioSettings = RadioSettingsT;
  using ActiveBandSettings = typename RadioSettingsT::ActiveBandSettings;
  using BandSettings = typename RadioSettingsT::BandSettings;

  SingleIqRxT(
    ComplexPingPongBuffers& pingPongBuffers,
    const BandCategoryList& bands,
    const ModeList& modes,
    IRadioPublishers* radioPublishers
    )
    : m_pingPongBuffers(pingPongBuffers)
    , m_rxPipelineA(PipelineId::A, modes, radioPublishers)
    , m_mixer(m_iqIo)
  {
    m_rxPipelineA.enableMonitoring(true);
    m_rxPipelineA.enableMetering(true);
  }
  ~SingleIqRxT() override
  {
    SingleIqRxT::stop(); // Joins the IQ source thread. Idempotent if already stopped.
  }

  ResultCode configure(const Config::IqReceiver::Fields& iqReceiverConfig) override
  {
    ResultCode rc = m_iqIo.configure(iqReceiverConfig.iqIo);
    if (rc != ResultCode::OK) return rc;

    m_rxPipelineA.initialise(&m_iqIo, &m_mixer.inputA());
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
  ResultCode apply(BandSettings* bandSettings)
  {
    const BandRfSettings* bandRfSettings = bandSettings->rfSettings(); // Always get these for RF offset calcs.
    // if (dualPipelineChanged) {
    if (bandSettings->hasfocusPipeline()) {
      RxPipelineSettings* rxPipelineASettings = bandSettings->focusPipeline();
      m_rxPipelineA.apply(bandRfSettings, rxPipelineASettings);
    }
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
    return out;
  }

  ResultCode ptt(bool on) override
  {
    return ResultCode::OK;
  }

protected:
  IqIo m_iqIo;
  ComplexPingPongBuffers& m_pingPongBuffers;
  IqRxPipeline m_rxPipelineA;
  AudioMixer m_mixer; // TODO: Get rid of this for single IQ pipeline
};
