#pragma once
#include <cstdint>
#include <utility>

#include <settings/model/message/PayloadType.h>

#include "MessageRingT.h"
#include "IMessagePublisherT.h"
#include "IMessageSubscriberT.h"


//*****************************************************************************
// A MessageRingT paired with a notifier that wakes the consumer.
//
// The producer reserves and commits slots exactly as with the ring; committing
// also invokes the notifier with the message's payload type. The notifier is
// platform-specific (e.g. QtNotifier posts a MessageEvent whose event type is
// the payload type); it may use the type to identify the channel, or ignore it.
//
// A wake means "check the channel", not "one message arrived": one wake may
// find several messages or none, so consumers must always drain until empty
// and tolerate finding nothing.
//*****************************************************************************
template<typename T, uint8_t N, typename NotifierT>
class MessageChannelT
{
public:
  using MessageType = T;

  using Publisher = IMessagePublisherT<T>;
  using Subscriber = IMessageSubscriberT<T>;

  // Arguments construct the notifier in place, so it need not be copyable or
  // movable (QtNotifier holds a mutex).
  template<typename... NotifierArgs>
  explicit MessageChannelT(NotifierArgs&&... notifierArgs)
    : m_notifier(std::forward<NotifierArgs>(notifierArgs)...)
  {
  }

  // Both threads hold references to the one instance; a copy would be a
  // different, disconnected channel.
  MessageChannelT(const MessageChannelT&) = delete;
  MessageChannelT& operator=(const MessageChannelT&) = delete;

  // For attaching and detaching the consumer, where the notifier supports it.
  NotifierT& notifier() { return m_notifier; }

  // Call after attaching a consumer. Messages committed while nothing was
  // attached had their notifications dropped, and if they filled the ring the
  // producer can commit nothing more to trigger another: without this wake the
  // channel would stay stuck. A commit racing the attach may cause a second
  // wake, which consumers already tolerate.
  void wakeIfPending()
  {
    if (pending() > 0) {
      m_notifier(static_cast<PayloadType>(T::payloadType));
    }
  }

  //--- producer thread only --------------------------------------------------

  [[nodiscard]] T* reserveWrite() { return m_ring.reserveWrite(); }

  void commitWrite(T* pmsg)
  {
    // Read before committing: once committed, the slot belongs to the consumer.
    const PayloadType payloadType = pmsg->getPayloadType();
    m_ring.commitWrite(pmsg);
    m_notifier(payloadType);
  }

  //--- consumer thread only --------------------------------------------------

  // Hands each pending message to handler, returning every slot to
  // circulation whether or not the handler acted on it.
  template<typename HandlerT>
  void consumePending(HandlerT&& handler)
  {
    while (T* pmsg = m_ring.reserveRead()) {
      handler(*pmsg);
      m_ring.commitRead(pmsg);
    }
  }

  [[nodiscard]] size_t pending() const { return m_ring.pending(); }

private:
  MessageRingT<T, N> m_ring;
  NotifierT          m_notifier;
};