#pragma once
#include <cstdint>
#include <type_traits>
#include <utility>

#include <settings/model/message/PayloadType.h>

#include "MessageRingBufferT.h"
#include "IMessagePublisherT.h"
#include "IMessageSubscriberT.h"


//*****************************************************************************
// A ring of message slots paired with a notifier that wakes the consumer.
//
// The producer reserves and commits slots; committing also invokes the
// notifier with the ring's payload type. The notifier is platform-specific
// (e.g. QtNotifier posts a MessageEvent whose event type is the payload type);
// it may use the type to identify the channel, or ignore it.
//
// A wake means "check the channel", not "one message arrived": one wake may
// find several messages or none, so consumers must always drain until empty
// and tolerate finding nothing.
//
// Slots hold only T::PayloadProto, the message's plain nanopb struct, never a
// whole T. The ring therefore contains no vtable pointers and no pointers to
// either end's objects, so the same ring works between threads (Qt, FreeRTOS)
// and between separately built program images sharing memory (the STM32H745's
// CM7 and CM4). Producers and consumers normally go through MessageRingSinkT
// and MessageRingSourceT, which keep a T at each end and copy the payload.
//*****************************************************************************
template<typename T, uint8_t N, typename NotifierT>
class MessageRingT
{
public:
  using MessageType = T;
  using SlotType = typename T::PayloadProto;

  using Publisher = IMessagePublisherT<T>;
  using Subscriber = IMessageSubscriberT<T>;

  static_assert(std::is_trivially_copyable_v<SlotType>,
                "Ring slots need a trivially copyable T::PayloadProto");
  static_assert(!std::is_polymorphic_v<NotifierT>,
                "A notifier must not have virtual functions (the ring may be shared)");
  static_assert(!std::is_polymorphic_v<MessageRingBufferT<SlotType, N>>,
                "Ring buffer has a vtable: is ETL_POLYMORPHIC_* defined?");

  // Arguments construct the notifier in place, so it need not be copyable or
  // movable (QtNotifier holds a mutex).
  template<typename... NotifierArgs>
  explicit MessageRingT(NotifierArgs&&... notifierArgs)
    : m_notifier(std::forward<NotifierArgs>(notifierArgs)...)
  {
  }

  // Both ends hold references to the one instance; a copy would be a
  // different, disconnected channel.
  MessageRingT(const MessageRingT&) = delete;
  MessageRingT& operator=(const MessageRingT&) = delete;

  NotifierT& notifier() { return m_notifier; }

  //--- attaching the recipient (only where NotifierT supports it) ------------

  // The target is woken by each subsequent commit. Messages committed while
  // nothing was attached had their notifications dropped, so it is also woken
  // now if any are waiting.
  //
  // A notifier that needs to know the ring's payload type when attaching
  // (one whose wake arrives without it, e.g. a hardware interrupt) takes it
  // as attach(target, payloadType).
  template<typename TargetT>
  void attachRecipient(TargetT target)
  {
    if constexpr (requires { m_notifier.attach(target, payloadType()); }) {
      m_notifier.attach(target, payloadType());
    } else {
      m_notifier.attach(target);
    }
    wakeIfPending();
  }

  void detachRecipient() { m_notifier.detach(); }

  //--- producer only ---------------------------------------------------------

  [[nodiscard]] SlotType* reserveWrite() { return m_ringBuffer.reserveWrite(); }

  void commitWrite(SlotType* pslot)
  {
    m_ringBuffer.commitWrite(pslot);
    m_notifier(payloadType());
  }

  //--- consumer only ---------------------------------------------------------

  // Hands each pending slot to handler, returning every slot to circulation
  // whether or not the handler acted on it.
  template<typename HandlerT>
  void consumePending(HandlerT&& handler)
  {
    while (SlotType* pslot = m_ringBuffer.reserveRead()) {
      handler(*pslot);
      m_ringBuffer.commitRead(pslot);
    }
  }

  [[nodiscard]] size_t pending() const { return m_ringBuffer.pending(); }

private:
  static constexpr PayloadType payloadType() { return static_cast<PayloadType>(T::payloadType); }

  // If messages filled the ring while nothing was attached, the producer can
  // commit nothing more to trigger a notification: without this wake the
  // channel would stay stuck. A commit racing the attach may cause a second
  // wake, which consumers already tolerate.
  void wakeIfPending()
  {
    if (pending() > 0) {
      m_notifier(payloadType());
    }
  }

  MessageRingBufferT<SlotType, N> m_ringBuffer;
  NotifierT                       m_notifier;
};
