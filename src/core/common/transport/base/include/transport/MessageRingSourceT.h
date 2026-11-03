#pragma once

#include <optional>
#include <type_traits>

#include <ResultCode.h>
#include <settings/model/message/MessageSourceT.h>
#include "IMessageRingConsumer.h"

//*****************************************************************************
// The consumer end of a channel, presented as an ordinary MessageSourceT.
//
// Call consumePending() when woken by the channel's notifier. Each pending
// payload is rebuilt into a local message with T's payload constructor, which
// also rebuilds any views T keeps over its own payload (e.g. ModeList), and
// passed to the connected sink, in order. The sink must not keep the pointer
// after applyMessage() returns.
//
// With no sink connected, pending messages are discarded so that their slots
// return to circulation.
//
// Consumer only.
//*****************************************************************************
template<typename ChannelT>
class MessageRingSourceT
  : public MessageSourceT<typename ChannelT::MessageType>
  , public IMessageRingConsumer
{
public:
  using MessageType = typename ChannelT::MessageType;
  using SlotType = typename ChannelT::SlotType;

  static_assert(std::is_constructible_v<MessageType, const SlotType&>,
                "T must be constructible from const T::PayloadProto&");

  explicit MessageRingSourceT(ChannelT& channel)
    : m_channel(channel)
  {
  }

  void connectMessageSink(MessageSinkT<MessageType>* sink) override
  {
    m_pSink = sink;
  }

  void consumePending() override
  {
    m_channel.consumePending([this](const SlotType& payload) {
      // Constructed rather than assigned: operator= may have semantics of its
      // own (settings mark every field present) already applied by the sink.
      m_local.emplace(payload);
      notifyMessage(&*m_local);
    });
  }

  // Where the channel's notifier supports it, target is woken when messages
  // are waiting; it should then call consumePending().
  template<typename TargetT>
  void attachRecipient(TargetT target) { m_channel.attachRecipient(target); }
  void detachRecipient()               { m_channel.detachRecipient(); }

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
  std::optional<MessageType> m_local;
};
