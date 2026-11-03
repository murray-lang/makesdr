#pragma once

#include <etl/array.h>
#include <etl/error_handler.h>

#include <ResultCode.h>
#include <settings/model/message/MessageSinkT.h>
#include "IMessagePublisherT.h"

//*****************************************************************************
// The producer end of a channel, presented as an ordinary MessageSinkT.
//
// applyMessage() copies the message into a local message with T's own
// assignment, then copies its payload into a free slot and commits it, which
// notifies the consumer. It never blocks: if the consumer has fallen behind
// and the ring is full, the message is dropped and ERR_CHANNEL_FULL returned.
//
// reserve() and commit() let a producer build a message in place: reserve()
// takes a slot and hands out the local message to fill; commit() copies its
// payload into that slot. Having already taken the slot, commit() cannot find
// the ring full.
//
// Producer only.
//*****************************************************************************
template<typename RingT>
class MessageRingSinkT :
  public IMessagePublisherT<typename RingT::MessageType>,
  public MessageSinkT<typename RingT::MessageType>
{
public:
  using MessageType = typename RingT::MessageType;
  using SlotType = typename RingT::SlotType;

  explicit MessageRingSinkT(RingT& channel)
    : m_ring(channel)
  {
  }

  // Returns nullptr if the ring is full. Idempotent until commit().
  MessageType* reserve() override
  {
    return m_ring.reserveWrite() != nullptr ? &m_local : nullptr;
  }

  // message must be what reserve() returned.
  void commit(MessageType* message) override
  {
    ETL_ASSERT(message == &m_local, ETL_ERROR(etl::array_out_of_range));
    SlotType* pslot = m_ring.reserveWrite();   // the slot reserve() took
    ETL_ASSERT(pslot != nullptr, ETL_ERROR(etl::array_out_of_range));
    *pslot = message->payload();
    m_ring.commitWrite(pslot);
  }

  ResultCode applyMessage(MessageType* message) override
  {
    SlotType* pslot = m_ring.reserveWrite();
    if (pslot == nullptr) {
      return ResultCode::ERR_CHANNEL_FULL;
    }
    m_local = *message;
    if constexpr (requires { m_local.payload().header; }) {
      // Some messages' operator= copies only the body (settings use replace()),
      // but the header's source and purpose must travel with the message.
      m_local.payload().header = message->payload().header;
    }
    *pslot = m_local.payload();
    m_ring.commitWrite(pslot);
    return ResultCode::OK;
  }

private:
  RingT&      m_ring;
  MessageType m_local;
};
