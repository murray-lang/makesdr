#pragma once

#include <QEvent>
#include <EventId.h>

struct QtEventRegistrar {

  QEvent::Type operator()() const
  {
    return static_cast<QEvent::Type>(QEvent::registerEventType());
  }
};