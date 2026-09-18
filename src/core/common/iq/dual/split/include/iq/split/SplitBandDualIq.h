#pragma once

#include <iq/base/IqRxTxBaseT.h>
#include <iq/io/IqIo.h>
#include <iq/pipeline/IqTxPipeline.h>
#include <settings/model/radios/iq/SplitBandDualIqRxTxSettings.h>
#include <iq/common/IqTransmitter.h>

#include "SplitBandDualIq_Rx.h"



class SplitBandDualIq : public IqRxTxBaseT<SplitBandDualIqRxTxSettings>
{
public:
  using RadioSettings = SplitBandDualIqRxTxSettings;
  using ActiveBandSettings = SplitBandDualIqRxTxSettings::ActiveBandSettings;
  using BandSettings = SplitBandDualIqRxTxSettings::BandSettings;

  SplitBandDualIq(const BandCategoryList& bands, const ModeList& modes);
  ~SplitBandDualIq() override = default;

  ResultCode configure(const Config::Sdr::Fields& sdrConfig) override;

  ResultCode start() override;
  void stop() override;

  ResultCode ptt(bool on) override;

  ResultCode apply(RadioSettings& settings) override;

protected:
  SplitBandDualIq_Rx m_rx;
  IqTransmitter m_tx;
};
