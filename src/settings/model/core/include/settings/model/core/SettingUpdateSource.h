#pragma once
#include "ResultCode.h"
#include "SettingUpdateSink.h"

class SettingUpdateSource
{
public:
  virtual ~SettingUpdateSource() = default;

  virtual void connectSettingUpdateSink(SettingUpdateSink& sink)
  {
    // This implementation is to avoid the need for a descendant class to implement an empty
    // connect() for this base class.
  }
protected:
  virtual ResultCode notifySettingUpdate(const SettingUpdate& settingUpdate) = 0;

};
