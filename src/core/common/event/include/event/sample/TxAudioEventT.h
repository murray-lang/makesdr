#pragma once

#include "AudioEventData.h"

#include "../DataEventBaseT.h"
#include <EventId.h>

template<typename EventBaseT, typename EventIdT>
class TxAudioEventT : public AudioEventData, public DataEventBaseT<EventBaseT, EventIdT, EVENT_AUDIO_TX>
{
public:
  TxAudioEventT(const RealSamplesMax& audio, uint32_t length, uint32_t sampleRate)
    : DataEventBaseT<EventBaseT, EventIdT, EVENT_AUDIO_TX>()
    , AudioEventData(audio, length, sampleRate)
  {}
};