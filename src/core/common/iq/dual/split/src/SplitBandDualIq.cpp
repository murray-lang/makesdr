#include "iq/split/SplitBandDualIq.h"

SplitBandDualIq::SplitBandDualIq(const BandCategoryList& bands, const ModeList& modes)
  : IqRxTxBaseT()
  , m_rx(bands, modes)
  , m_tx(modes)
{

}

ResultCode
SplitBandDualIq::configure(const Config::Sdr::Fields& sdrConfig)
{
  ResultCode rc = m_rx.configure(sdrConfig);
  if (rc != ResultCode::OK)  return rc;
  return m_tx.configure(sdrConfig);
}

ResultCode
SplitBandDualIq::start()
{
  ResultCode rc = m_rx.start();
  if (rc != ResultCode::OK) return rc;
  return m_tx.start();
}

void
SplitBandDualIq::stop()
{
  m_tx.stop();
  m_rx.stop();
}

ResultCode
SplitBandDualIq::ptt(bool on)
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

ResultCode
SplitBandDualIq::apply(RadioSettings& settings)
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