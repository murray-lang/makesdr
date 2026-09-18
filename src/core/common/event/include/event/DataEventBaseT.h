#pragma once
#include <EventId.h>

template<typename EventBaseT, typename EventIdT, EventId _eventId>
class DataEventBaseT : public EventBaseT
{
public:
  static inline EventIdT eventId = static_cast<EventIdT>(_eventId);

  DataEventBaseT() : EventBaseT(eventId)
  {
  }
};
