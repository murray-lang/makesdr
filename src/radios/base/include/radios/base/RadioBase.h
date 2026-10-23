#pragma once
#include <settings/model/radio/RadioSettingsSinkT.h>
#include <settings/control/sink/PttSink.h>

#include <event/EventTarget.h>

#include "config/struct/RadioConfig.h"
#include "settings/model/update/FieldUpdateSink.h"

class RadioBase : public FieldUpdateSink, public PttSink
{
public:
  virtual ResultCode configure(const Config::Radio::Fields& config) = 0;
  virtual ResultCode start() = 0;
  virtual void stop() = 0;
};
