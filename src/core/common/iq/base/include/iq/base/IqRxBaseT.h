#pragma once

#include <ResultCode.h>
#include "IqSink.h"
#include <settings/control/sink/PttSink.h>
#include <config/struct/IqReceiverConfig.h>


#include <event/EventTarget.h>
// #include <settings/model/radio/BandSettings.h>

template<typename RadioSettingsT>
class IqRxBaseT : public IqSink, public PttSink // It's a PttSink so that it can respond by muting itself
{
public:
  IqRxBaseT() = default;
  ~IqRxBaseT() override = default;

  virtual ResultCode start() = 0;
  virtual void stop() = 0;
  virtual ResultCode configure(const Config::IqReceiver::Fields& iqReceiverConfig) = 0;
  virtual ResultCode apply(RadioSettingsT& radioSettings) = 0;
  // virtual ResultCode apply(const BandSettings& bandSettings) = 0;

};