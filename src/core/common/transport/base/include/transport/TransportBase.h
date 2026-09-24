#pragma once
#include <ResultCode.h>

class TransportBase
{
public:
  virtual ~TransportBase() = default;
  virtual ResultCode open() = 0;
  virtual void close() = 0;
};