#pragma once
#include <transport/IMessagePublisherT.h>
#include <settings/model/radio/iq/FftMessage.h>

using FftPublisher = IMessagePublisherT<FftMessage>;
