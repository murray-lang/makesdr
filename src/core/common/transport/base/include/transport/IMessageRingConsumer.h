#pragma once

class IMessageRingConsumer
{
public:
  enum Policy { POLICY_NONE, POLICY_ALL, POLICY_LATEST };
  virtual ~IMessageRingConsumer() = default;
  virtual void consumePending() = 0;
};