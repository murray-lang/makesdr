#pragma once
#include <EventId.h>

template<typename EventBaseT, typename EventIdT, EventId _eventId>
class PayloadEventBaseT : public EventBaseT
{
public:
  static inline const EventIdT eventId = static_cast<EventIdT>(_eventId);

  enum EventSource { ANY, FRONT_END, BACK_END };

  PayloadEventBaseT(EventSource source) : EventBaseT(eventId), m_source(source) {}

  [[nodiscard]] EventSource getSource() const { return m_source; }

protected:
  EventSource m_source;

};