#pragma once
#include <transport/IMessagePublisherT.h>
#include <settings/model/radio/iq/IqMessage.h>

using IqPublisher = IMessagePublisherT<IqMessage>;
