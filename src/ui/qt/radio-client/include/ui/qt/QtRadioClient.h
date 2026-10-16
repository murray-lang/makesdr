#pragma once
#include "QtRadioClientT.h"
// #include <settings/model/radio/RadioSettings.h>
#include <radios/settings/RadioSettingsVariant.h>
#include <radios/settings/model/SplitBandDualIqRxTxSettings.h>

using QtRadioClientBase = QtRadioClientT<SplitBandDualIqRxTxSettings>;

class QtRadioClient : public QObject, public QtRadioClientBase
{
  Q_OBJECT
public:
  using RadioSettings = SplitBandDualIqRxTxSettings;
  QtRadioClient(QObject* parent) : QtRadioClientBase(parent) {};
  ~QtRadioClient() override = default;
  signals:
    void capabilitiesReceived(const Capabilities* settings);
    void radioSettingsReceived(const RadioSettings* settings);
    void receiverIqReceived(const IqMessage* iq);
    void rxMeteringReceived(const RxMeteringMessage* rxMetering);
    void fftReceived(const FftMessage* fft);

protected:
  void emitCapabilitiesReceived(const Capabilities* caps) override
  {
    emit capabilitiesReceived(caps);
  }

  void emitRadioSettingsReceived(const RadioSettings* settings) override
  {
    emit radioSettingsReceived(settings);
  }

  void emitReceiverIqReceived(const IqMessage* iq) override
  {
    emit receiverIqReceived(iq);
  }

  void emitRxMeteringReceived(const RxMeteringMessage* rxMetering) override
  {
    emit rxMeteringReceived(rxMetering);
  }

  void emitFftReceived(const FftMessage* fft) override
  {
    emit fftReceived(fft);
  }

protected:
  RadioSettingsVariant m_settingsVariant;
};
