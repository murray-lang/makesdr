#pragma once

#include <iq/base/IqSink.h>
#include <samples/SampleTypes.h>

class MyIqSink : public IqSink
{
public:
  ~MyIqSink() override = default;
  uint32_t sinkIq(ComplexSamplesBuffer& samples, uint32_t length) override;
};