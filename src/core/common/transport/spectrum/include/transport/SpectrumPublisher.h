#pragma once
#include <transport/IMessagePublisherT.h>
#include <settings/model/radio/iq/SpectrumMessage.h>

using SpectrumPublisher = IMessagePublisherT<SpectrumMessage>;
