#pragma once

#include <CrossPlatformTypes.h>
#include <samples/SampleTypes.h>
#include "RxIqMeterEventData.h"
#include "../DataEventBaseT.h"
#include <EventId.h>

template<typename EventBaseT, typename EventIdT>
class RxIqMeterEventT :
  public RxIqMeterEventData,
  public DataEventBaseT<EventBaseT, EventIdT, EVENT_METER_RX_IQ>
{
public:
  RxIqMeterEventT(float rssiDbFs, uint32_t sampleRate, optional<float> agcGainDb = nullopt)
    : RxIqMeterEventData(rssiDbFs, agcGainDb, sampleRate)
    , DataEventBaseT<EventBaseT, EventIdT, EVENT_METER_RX_IQ>()
  {}

};