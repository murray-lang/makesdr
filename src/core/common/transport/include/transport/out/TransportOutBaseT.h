#pragma once

#include <settings/model/message/MessageSinkT.h>
#include "../TransportBase.h"

template<typename MessageType>
class TransportOutBaseT : public MessageSinkT<MessageType>, public TransportBase
{

};