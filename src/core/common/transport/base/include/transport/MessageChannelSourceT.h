#pragma once

#include <ResultCode.h>
#include <settings/model/message/MessageSourceT.h>
#include "IMessageChannelConsumer.h"

//*****************************************************************************
// The consumer end of a channel, presented as an ordinary MessageSourceT.
//
// Call consumePending() when woken by the channel's notifier. Each pending
// message is passed to the connected sink, in order, directly from its slot:
// the sink must not keep the pointer after applyMessage() returns.
//
// With no sink connected, pending messages are discarded so that their slots
// return to circulation.
//
// Consumer thread only.
//*****************************************************************************
template<typename ChannelT>
class MessageChannelSourceT
  : public MessageSourceT<typename ChannelT::MessageType>
  , public IMessageChannelConsumer
{
public:
  using MessageType = typename ChannelT::MessageType;

  explicit MessageChannelSourceT(ChannelT& channel)
    : m_channel(channel)
  {
  }

  void connectMessageSink(MessageSinkT<MessageType>* sink) override
  {
    m_pSink = sink;
  }

  void consumePending() override
  {
    m_channel.consumePending([this](MessageType& message) { notifyMessage(&message); });
  }

protected:
  ResultCode notifyMessage(MessageType* message) override
  {
    ResultCode rc = ResultCode::OK;
    if (m_pSink) {
      rc = m_pSink->applyMessage(message);
    }
    return rc;
  }

private:
  ChannelT&                  m_channel;
  MessageSinkT<MessageType>* m_pSink{nullptr};
};
