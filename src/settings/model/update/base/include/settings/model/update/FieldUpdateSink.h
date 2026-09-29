#pragma once

#include "ResultCode.h"
#include "FieldUpdate.h"

class FieldUpdateSink
{
public:
  virtual ~FieldUpdateSink() = default;

  virtual ResultCode applyFieldUpdate(const FieldUpdate& update) = 0;
};