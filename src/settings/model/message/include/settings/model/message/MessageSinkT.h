#pragma once

#include <ResultCode.h>

template<typename MessageType>
class MessageSinkT
{
public:
  virtual ~MessageSinkT() = default;
  virtual ResultCode applyMessage(MessageType* message) = 0;
};