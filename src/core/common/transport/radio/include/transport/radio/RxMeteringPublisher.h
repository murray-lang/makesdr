#pragma once
#include <transport/IMessagePublisherT.h>
#include <settings/model/radio/RxMeteringMessage.h>

using RxMeteringPublisher = IMessagePublisherT<RxMeteringMessage>;
