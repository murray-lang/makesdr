#pragma once

#include "AudioEventData.h"
#include "../DataEventBaseT.h"
#include <EventId.h>

template<typename EventBaseT, typename EventIdT>
class RxAudioEventT : public AudioEventData, public DataEventBaseT<EventBaseT, EventIdT, EVENT_AUDIO_RX>
{
public:
  RxAudioEventT(const RealSamplesMax& audio, uint32_t length, uint32_t sampleRate)
    : AudioEventData(audio, length, sampleRate)
    , DataEventBaseT<EventBaseT, EventIdT, EVENT_AUDIO_RX>()
  {}
};