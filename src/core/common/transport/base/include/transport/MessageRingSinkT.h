#pragma once

#include <ResultCode.h>
#include <settings/model/message/MessageSinkT.h>
#include "IMessagePublisherT.h"

//*****************************************************************************
// The producer end of a channel, presented as an ordinary MessageSinkT.
//
// applyMessage() copies the message into a free slot and commits it, which
// notifies the consumer. It never blocks: if the consumer has fallen behind
// and the ring is full, the message is dropped and ERR_CHANNEL_FULL returned.
//
// Producer thread only.
//*****************************************************************************
template<typename RingT>
class MessageRingSinkT :
  public IMessagePublisherT<typename RingT::MessageType>,
  public MessageSinkT<typename RingT::MessageType>
{
public:
  using MessageType = typename RingT::MessageType;


  explicit MessageRingSinkT(RingT& channel)
    : m_ring(channel)
  {
  }

  MessageType* reserve() override { return m_ring.reserveWrite(); }
  void commit(MessageType* message) override { m_ring.commitWrite(message); }

  ResultCode applyMessage(MessageType* message) override
  {
    MessageType* pmsg = m_ring.reserveWrite();
    if (pmsg == nullptr) {
      return ResultCode::ERR_CHANNEL_FULL;
    }
    *pmsg = *message;
    if constexpr (requires { pmsg->payload().header; }) {
      // Some messages' operator= copies only the body (settings use replace()),
      // but the header's source and purpose must travel with the message.
      pmsg->payload().header = message->payload().header;
    }
    m_ring.commitWrite(pmsg);
    return ResultCode::OK;
  }

private:
  RingT& m_ring;
};
