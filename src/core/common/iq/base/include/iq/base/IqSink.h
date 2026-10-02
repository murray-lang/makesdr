#pragma once
#include <cstdint>
#include <samples/SampleTypes.h>

class IqSink
{
public:
  virtual ~IqSink() = default;
  virtual uint32_t sinkIq(ComplexSamplesBuffer& samples, uint32_t length) = 0;
};
