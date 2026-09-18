#pragma once

#include <iq/base/IqRxBaseT.h>
#include <iq/io/IqIo.h>
#include <iq/pipeline/IqRxPipeline.h>
#include <settings/model/radios/iq/BasicIqRxSettings.h>


class BasicIqRx : public IqRxBaseT<BasicIqRxSettings>
{
public:
  using RadioSettings = BasicIqRxSettings;
  using ActiveBandSettings = BasicIqRxSettings::ActiveBandSettings;
  using BandSettings = BasicIqRxSettings::BandSettings;

  BasicIqRx(const ModeList& modes);
  ~BasicIqRx() override = default;

  ResultCode configure(const Config::IqReceiver::Fields& iqReceiverConfig) override;

  ResultCode start() override;
  void stop() override;

  ResultCode apply(BasicIqRxSettings& settings) override;
  // ResultCode apply(const BandSettings& bandSettings) override;

  uint32_t sinkIq(ComplexPingPongBuffers& samples, uint32_t length) override;

protected:
  IqIo m_iqIo;
  IqRxPipeline m_pipelineA;
};
