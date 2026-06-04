#pragma once

#include "settings/control/sink/PttSink.h"

class PttSource
{
public:
  virtual ~PttSource() = default;
  virtual void connect(PttSink* pSink) = 0;
};