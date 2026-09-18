#pragma once

#include <iq/base/IqRxTxBaseT.h>
#include <iq/io/IqIo.h>
#include <settings/model/radios/iq/BasicIqRxTxSettings.h>
#include <iq/common/IqTransmitter.h>

#include "BasicIqRxTx_Rx.h"


class BasicIqRxTx : public IqRxTxBaseT<BasicIqRxTxSettings>
{
public:
  using RadioSettings = BasicIqRxTxSettings;
  using ActiveBandSettings = BasicIqRxTxSettings::ActiveBandSettings;
  using BandSettings = BasicIqRxTxSettings::BandSettings;

  BasicIqRxTx(const ModeList& modes);
  ~BasicIqRxTx() override = default;

  ResultCode configure(const Config::Sdr::Fields& sdrConfig) override;

  ResultCode start() override;
  void stop() override;

  ResultCode apply(RadioSettings& settings) override;

protected:
  BasicIqRxTx_Rx m_rx;
  IqTransmitter m_tx;
};
