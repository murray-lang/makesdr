#pragma once

#include "MessageSinkT.h"

template<typename MessageType>
class MessageSourceT
{
public:
  virtual ~MessageSourceT() = default;
  virtual void connectMessageSink(MessageSinkT<MessageType>* sink) = 0;

protected:
  virtual ResultCode notifyMessage(MessageType* settings) = 0;
};

