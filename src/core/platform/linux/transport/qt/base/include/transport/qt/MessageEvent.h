#pragma once

#include <QEvent>

#include <settings/model/message/PayloadType.h>

class MessageEvent : public QEvent
{
public:
  explicit MessageEvent(PayloadType payloadType)
    : QEvent( static_cast<QEvent::Type>(QEvent::User + static_cast<QEvent::Type>(payloadType)))
  {
  }
};
