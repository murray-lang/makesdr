#pragma once
#include <ResultCode.h>
#include <settings/model/message/FieldUpdateSink.h>

class FieldUpdateSource
{
public:
  virtual ~FieldUpdateSource() = default;

  virtual void connectFieldUpdateSink(FieldUpdateSink* sink)
  {
    // This implementation is to avoid the need for a descendant class to implement an empty
    // connect() for this base class.
  }
protected:
  virtual ResultCode notifyFieldUpdate(const FieldUpdate& settingUpdate) = 0;

};
