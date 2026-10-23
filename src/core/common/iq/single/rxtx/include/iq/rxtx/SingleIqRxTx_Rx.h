#pragma once

#include <iq/base/IqRxTxBaseT.h>
#include <iq/io/IqIo.h>
#include <iq/pipeline/IqRxPipeline.h>


class SingleIqRxTx_Rx : public IqSink
{
public:
  SingleIqRxTx_Rx(const ModeList& modes);
  ~SingleIqRxTx_Rx() override = default;

  ResultCode configure(const Config::Sdr::Fields& sdrConfig);

  ResultCode start();
  void stop();

  ResultCode apply(const BandRfSettings* bandRfSettings, RxPipelineSettings* settings);

  uint32_t sinkIq(ComplexPingPongBuffers& samples, uint32_t length) override;

protected:
  IqIo m_iqIo;
  IqRxPipeline m_rxPipeline;
};
