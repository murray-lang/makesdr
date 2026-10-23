#pragma once

#include <iq/base/IqRxTxBaseT.h>
#include <iq/io/IqIo.h>
#include <iq/pipeline/IqTxPipeline.h>
#include <iq/common/IqTransmitter.h>

#include <radios/iq/DualIqRxT.h>
#include <transport/IRadioPublishers.h>

template<typename RadioSettingsT>
class DualIqRxTxT : public IqRxTxBaseT<RadioSettingsT>
{
public:
  using RadioSettings = RadioSettingsT;
  using ActiveBandSettings = typename RadioSettingsT::ActiveBandSettings;
  using BandSettings = typename RadioSettingsT::BandSettings;

  DualIqRxTxT(
    const BandCategoryList& bands,
    const ModeList& modes,
    IRadioPublishers* radioPublishers
    )
    : IqRxTxBaseT<RadioSettingsT>()
    , m_rx(m_pingPongBuffers, bands, modes, radioPublishers)
    , m_tx(m_pingPongBuffers, modes)
  {

  }
  ~DualIqRxTxT() override = default;

  ResultCode configure(const Config::Sdr::Fields& sdrConfig) override
  {
    if (sdrConfig.receiver) {
      ResultCode rc = m_rx.configure(sdrConfig.receiver.value());
      if (rc != ResultCode::OK) return rc;
    } else {
      return ResultCode::ERR_CONFIG_RXTX_NO_RX;
    }
    return m_tx.configure(sdrConfig);
  }

  ResultCode start() override
  {
    return m_rx.start();
    // if (rc != ResultCode::OK) return rc;
    // return m_tx.start();
  }

  void stop() override
  {
    m_tx.stop();
    m_rx.stop();
  }

  ResultCode ptt(bool on) override
  {
    if (on) {
      m_rx.stop();
      m_tx.start();
    } else {
      m_tx.stop();
      m_rx.start();
    }
    return ResultCode::OK;
  }


  ResultCode apply(RadioSettings& settings) override
  {
    ResultCode rc = ResultCode::OK;
    if (settings.hasActiveBands()) {
      ActiveBandSettings* activeBandSettings = settings.activeBands();

      if (activeBandSettings != nullptr && activeBandSettings->hasFocusBand()) {
        BandSettings* bandSettings = activeBandSettings->focusBandSettings();
        rc = m_rx.apply(bandSettings);
        if (rc != ResultCode::OK) return rc;

        BandRfSettings* bandRfSettings = bandSettings->hasRfSettings() ? bandSettings->rfSettings() : nullptr;
        if (bandSettings->hasTxPipeline()) {
          TxPipelineSettings* txPipelineSettings = bandSettings->txPipeline();
          rc = m_tx.apply(bandRfSettings, &txPipelineSettings->base());
        }

      }
    }
    return rc;
  }

protected:
  ComplexPingPongBuffers m_pingPongBuffers;
  DualIqRxT<RadioSettings> m_rx;
  IqTransmitter m_tx;
};
