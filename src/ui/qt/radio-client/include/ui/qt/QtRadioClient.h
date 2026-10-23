#pragma once
#include "QtRadioClientT.h"
// #include <settings/model/radio/RadioSettings.h>
#include <radios/settings/RadioSettingsVariant.h>
#include <settings/model/radio/iq/SplitBandDualIqRxTxSettings.h>

// using QtRadioClientBase = QtRadioClientT<SplitBandDualIqRxTxSettings>;

class QtRadioClient : public QObject, public QtRadioClientT
{
  Q_OBJECT
public:
  QtRadioClient(QObject* parent) : QtRadioClientT(parent) {};
  ~QtRadioClient() override = default;
  signals:
    void capabilitiesReceived(const Capabilities* settings);
    // Views only what changed (see has_*), valid only during emission: connect directly, not queued.
    void radioSettingsReceived(const IRadioSettingsView* changes);
    void rxMeteringReceived(const RxMeteringMessage* rxMetering);
    void spectrumReceived(const SpectrumMessage* fft);

protected:
  void emitCapabilitiesReceived(const Capabilities* caps) override
  {
    emit capabilitiesReceived(caps);
  }

  void emitRadioSettingsReceived(const IRadioSettingsView* settings) override
  {
    emit radioSettingsReceived(settings);
  }

  void emitRxMeteringReceived(const RxMeteringMessage* rxMetering) override
  {
    emit rxMeteringReceived(rxMetering);
  }

  void emitSpectrumReceived(const SpectrumMessage* spectrum) override
  {
    emit spectrumReceived(spectrum);
  }

protected:
};
