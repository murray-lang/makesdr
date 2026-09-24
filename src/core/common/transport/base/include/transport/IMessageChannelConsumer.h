#pragma once

class IMessageChannelConsumer
{
public:
  enum Policy { POLICY_NONE, POLICY_ALL, POLICY_LATEST };
  virtual ~IMessageChannelConsumer() = default;
  virtual void consumePending() = 0;
};