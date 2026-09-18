#pragma once
#include <ResultCode.h>

class PttSink
{
public:
  virtual ~PttSink() = default;
  virtual ResultCode ptt(bool on) = 0;
};
