#pragma once

#include <CrossPlatformTypes.h>
#include <samples/SampleTypes.h>
#include "RxMeterEventData.h"
#include "../DataEventBaseT.h"

template<typename EventBaseT, typename EventIdT>
class RxMeterEventT : public RxMeterEventData, public DataEventBaseT<EventBaseT, EventIdT, EVENT_METER_RX>
{
public:
  RxMeterEventT(float rssiDbFs, optional<float> agcGainDb = nullopt)
    : RxMeterEventData(rssiDbFs, agcGainDb)
    , DataEventBaseT<EventBaseT, EventIdT, EVENT_METER_RX>()
  {}
};