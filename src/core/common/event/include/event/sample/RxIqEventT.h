#pragma once

#include "IqEventData.h"
#include "../DataEventBaseT.h"
#include <EventId.h>


template<typename EventBaseT, typename EventIdT>
class RxIqEventT : public IqEventData, public DataEventBaseT<EventBaseT, EventIdT, EVENT_IQ_RX>
{
public:
  RxIqEventT(const ComplexSamplesMax& iq, uint32_t length, uint32_t sampleRate)
    : IqEventData(iq, length, sampleRate)
    , DataEventBaseT<EventBaseT, EventIdT, EVENT_IQ_RX>()
  {}
};