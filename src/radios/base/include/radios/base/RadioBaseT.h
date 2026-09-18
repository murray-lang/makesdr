#pragma once
#include <settings/model/radios/IRadioSettings.h>
#include <settings/model/radios/RadioSettingsSinkT.h>
#include <settings/control/sink/PttSink.h>

#include <event/EventTarget.h>

#include "config/struct/RadioConfig.h"
#include "settings/model/message/FieldUpdateSink.h"

template <typename RadioSettingsT>
class RadioBaseT : public RadioSettingsSinkT<RadioSettingsT>, public FieldUpdateSink, public PttSink
{
public:
  virtual ResultCode configure(const Config::Radio::Fields& config) = 0;
  virtual ResultCode start() = 0;
  virtual void stop() = 0;

  [[nodiscard]] virtual const IRadioSettings* getSettings() const = 0;
  // [[nodiscard]] virtual const RadioLookup* getLookup() const = 0;
};
