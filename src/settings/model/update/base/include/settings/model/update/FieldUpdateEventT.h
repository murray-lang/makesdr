#pragma once
#include "PayloadEventBaseT.h"
#include "FieldUpdate.h"
#include <EventId.h>

template<typename EventBaseT, typename EventIdT>
class FieldUpdateEventT : public PayloadEventBaseT<EventBaseT, EventIdT, EVENT_SETTINGS_UPDATE>
{
public:
  using Base = PayloadEventBaseT<EventBaseT, EventIdT, EVENT_SETTINGS_UPDATE>;

  FieldUpdateEventT(Base::EventSource source, const FieldUpdate& update)
    : Base(source)
    , m_update(update)
    , m_final(final)
  {}

  template<typename... Args>
  FieldUpdateEventT(Base::EventSource source, Args&&... args)
    : Base(source)
    , m_update(::forward<Args>(args)...)
    , m_final(final)
  {}

  [[nodiscard]] const FieldUpdate& getUpdate() const { return m_update; }
  [[nodiscard]] bool isFinal() const { return m_final; }

protected:
  FieldUpdate m_update;
  bool m_final;
};
