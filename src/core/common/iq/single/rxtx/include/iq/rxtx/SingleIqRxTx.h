#pragma once

#include <iq/base/IqRxTxBaseT.h>
#include <iq/io/IqIo.h>
#include <settings/model/radio/iq/SingleIqRxTxSettings.h>
#include <iq/common/IqTransmitter.h>

#include "SingleIqRxTx_Rx.h"


class SingleIqRxTx : public IqRxTxBaseT<SingleIqRxTxSettings>
{
public:
  using RadioSettings = SingleIqRxTxSettings;
  using ActiveBandSettings = SingleIqRxTxSettings::ActiveBandSettings;
  using BandSettings = SingleIqRxTxSettings::BandSettings;

  SingleIqRxTx(const ModeList& modes);
  ~SingleIqRxTx() override = default;

  ResultCode configure(const Config::Sdr::Fields& sdrConfig) override;

  ResultCode start() override;
  void stop() override;

  ResultCode apply(RadioSettings& settings) override;

protected:
  SingleIqRxTx_Rx m_rx;
  IqTransmitter m_tx;
};
