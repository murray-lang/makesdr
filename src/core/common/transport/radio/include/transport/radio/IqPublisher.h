#pragma once
#include <transport/IMessagePublisherT.h>
#include <settings/model/radios/iq/IqMessage.h>

using IqPublisher = IMessagePublisherT<IqMessage>;
