#pragma once
#include <ResultCode.h>
#include <CrossPlatformTypes.h>
#include <settings/model/message/MessageSourceT.h>
#include "../TransportBase.h"



template<typename MessageType>
class TransportInBaseT : public MessageSourceT<MessageType>, public TransportBase
{
public:
  TransportInBaseT() = default;
  ~TransportInBaseT() override = default;

  void connectMessageSink(MessageSinkT<MessageType>* sink) override
  {
    m_pSink = sink;
  }

protected:
  ResultCode notifyMessage(MessageType* message) override
  {
    if (m_pSink) {
      return m_pSink->applyMessage(message);
    }
    return ResultCode::OK;
  }

  MessageSinkT<MessageType>* m_pSink{nullptr};
};
