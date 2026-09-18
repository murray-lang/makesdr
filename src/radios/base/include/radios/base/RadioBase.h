#pragma once
#include <config/struct/RadioConfig.h>
#include <settings/model/radios/RadioSettingsSink.h>
#include <settings/control/sink/PttSink.h>

#include <event/EventTarget.h>

class RadioBase : public RadioSettingsSink, public FieldUpdateSink, public PttSink
{
public:
  virtual ResultCode configure(const Config::Radio::Fields& config) = 0;
  virtual ResultCode start() = 0;
  virtual void stop() = 0;

  [[nodiscard]] virtual const IRadioSettings* getSettings() const = 0;
  [[nodiscard]] virtual const BandCategoryList* getBands() const = 0;
  [[nodiscard]] virtual const ModeList* getModes() const = 0;
};
