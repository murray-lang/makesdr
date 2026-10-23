#pragma once

#include <EventId.h>
#include <settings/model/message/PayloadEventBaseT.h>

template<typename RadioSettingsT, typename EventBaseT, typename EventIdT, EventId eventId>
class RadioSettingsEventT :
  public PayloadEventBaseT<EventBaseT, EventIdT, eventId>
{
public:
  using Base = PayloadEventBaseT<EventBaseT, EventIdT, eventId>;

  explicit RadioSettingsEventT(
    const RadioSettingsT& radioSettings,
    int32_t sequence,
    Base::EventSource source
    )
    : Base(source)
    , m_radioSettings(::move(radioSettings))
    , m_sequence(sequence)
  {
  }
  [[nodiscard]] const RadioSettingsT& getRadioSettings() const { return m_radioSettings; }
  [[nodiscard]] int32_t getSequence() const { return m_sequence; }

protected:
  RadioSettingsT m_radioSettings;
  int32_t m_sequence;
};

