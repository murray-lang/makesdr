#pragma once

#include <iq/base/IqRxTxBaseT.h>
#include <iq/io/IqIo.h>
#include <iq/pipeline/IqTxPipeline.h>
#include <iq/common/IqTransmitter.h>
#include <settings/model/radios/iq/DualIqRxTxSettings.h>
#include "DualIqRxTx_Rx.h"


class DualIqRxTx : public IqRxTxBaseT<DualIqRxTxSettings>
{
public:
  using RadioSettings = DualIqRxTxSettings;
  using ActiveBandSettings = DualIqRxTxSettings::ActiveBandSettings;
  using BandSettings = DualIqRxTxSettings::BandSettings;

  DualIqRxTx(const ModeList& modes);
  ~DualIqRxTx() override = default;

  ResultCode configure(const Config::Sdr::Fields& sdrConfig) override;

  ResultCode start() override;
  void stop() override;

  void ptt(bool on) override;

  ResultCode apply(RadioSettings& settings) override;

protected:
  DualIqRxTx_Rx m_rx;
  IqTransmitter m_tx;
};
