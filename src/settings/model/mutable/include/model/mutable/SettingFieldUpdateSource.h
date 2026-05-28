#pragma once
#include "ResultCode.h"
#include "SettingFieldUpdateSink.h"

class SettingFieldUpdateSource
{
public:
  virtual ~SettingFieldUpdateSource() = default;

  virtual void connectSettingFieldUpdateSink(SettingFieldUpdateSink& pSink)
  {
    // This implementation is to avoid the need for a descendant class to implement an empty
    // connect() for this base class.
  }
protected:
  virtual ResultCode notifySettingFieldUpdate(const SettingFieldUpdate& settingUpdate) = 0;

};
