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
template<typename ChannelT>
class MessageChannelSinkT :
  public IMessagePublisherT<typename ChannelT::MessageType>,
  public MessageSinkT<typename ChannelT::MessageType>
{
public:
  using MessageType = typename ChannelT::MessageType;


  explicit MessageChannelSinkT(ChannelT& channel)
    : m_channel(channel)
  {
  }

  MessageType* reserve() override { return m_channel.reserveWrite(); }
  void commit(MessageType* message) override { m_channel.commitWrite(message); }

  ResultCode applyMessage(MessageType* message) override
  {
    MessageType* pmsg = m_channel.reserveWrite();
    if (pmsg == nullptr) {
      return ResultCode::ERR_CHANNEL_FULL;
    }
    *pmsg = *message;
    if constexpr (requires { pmsg->payload().header; }) {
      // Some messages' operator= copies only the body (settings use replace()),
      // but the header's source and purpose must travel with the message.
      pmsg->payload().header = message->payload().header;
    }
    m_channel.commitWrite(pmsg);
    return ResultCode::OK;
  }

private:
  ChannelT& m_channel;
};
