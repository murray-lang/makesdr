#pragma once

#include <QEvent>

template<typename MessageType>
class MessageEventT : public QEvent
{
public:
  MessageEventT(const MessageType& message)
    : QEvent( static_cast<QEvent::Type>(QEvent::User + MessageType::payloadType))
    , payload(message.payload())
    , complete(message.deemComplete())
  {
  }
  MessageType::PayloadProto payload;
  bool complete;
};