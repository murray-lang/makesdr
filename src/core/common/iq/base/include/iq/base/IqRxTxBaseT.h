#pragma once

#include <ResultCode.h>
// #include <iq/base/IqSink.h>
// #include <audio/AudioSink.h>
#include <config/struct/SdrConfig.h>
#include <settings/control/sink/PttSink.h>
#include <settings/model/radios/IRadioSettings.h>

#include <event/EventTarget.h>

template<typename RadioSettingsT>
class IqRxTxBaseT : public PttSink  //: public IqSink, public AudioSink
{
public:
  IqRxTxBaseT() = default;
  ~IqRxTxBaseT() override = default;

  virtual ResultCode start() = 0;
  virtual void stop() = 0;
  virtual ResultCode configure(const Config::Sdr::Fields& sdrConfig) = 0;
  virtual ResultCode apply(RadioSettingsT& radioSettings) = 0;

protected:

};
