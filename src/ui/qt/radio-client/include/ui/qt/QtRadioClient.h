#pragma once
#include "QtRadioClientT.h"
#include <settings/model/radios/iq/SplitBandDualIqRxTxSettings.h>

using QtRadioClientBase = QtRadioClientT<SplitBandDualIqRxTxSettings>;

class QtRadioClient : public QObject, public QtRadioClientBase
{
  Q_OBJECT
public:
  using RadioSettings = SplitBandDualIqRxTxSettings;
  QtRadioClient(QObject* parent) : QtRadioClientBase(parent) {};
  ~QtRadioClient() override = default;
  signals:
    void radioSettingsReceived(const RadioSettings* settings);

protected:
  void emitRadioSettingsReceived(const RadioSettings* settings) override
  {
    emit radioSettingsReceived(settings);
  }
};
