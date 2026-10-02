#pragma once

#include <cstdint>
#include <etl/array.h>
#include <etl/queue_spsc_atomic.h>
#include <etl/error_handler.h>

//*****************************************************************************
// A fixed-capacity, lock-free, single-producer / single-consumer ring of
// preallocated message slots.
//
// Slots circulate in one direction only:
//
//   m_free --pop--> producer --push--> m_ready --pop--> consumer --push--> m_free
//           (P)                 (P)               (C)                (C)
//
// Each queue therefore has exactly one writer and one reader. Nothing may move
// a slot against that circuit; there is deliberately no "discard" or "return"
// operation on either side.
//
// Both reserves are idempotent and non-destructive: reserving twice without an
// intervening commit yields the same slot, and abandoning a reservation costs
// nothing. Only the commits advance anything.
//
// Each side may hold at most one reservation at a time.
//*****************************************************************************
template<typename T, uint8_t N>
class MessageRingT
{
public:
  using Tag = uint8_t;
  static constexpr Tag NoTag = N;          // N is never a valid slot index

  static_assert(N > 0, "MessageRingT needs at least one slot");

  MessageRingT()
    : m_writeReservation(NoTag)
    , m_readReservation(NoTag)
  {
    // Safe as the sole third writer to m_free only because this runs before
    // either thread exists. On an AMP target the ring must be constructed by
    // exactly one core, with the other blocked until the handshake completes.
    for (Tag i = 0; i < N; ++i) {
      m_free.push(i);
    }
  }

  //--- producer thread only --------------------------------------------------

  // Returns the reserved slot, or nullptr if the ring is full. Idempotent:
  // returns the same slot until commitWrite(). Abandoning costs nothing.
  [[nodiscard]] T* reserveWrite()
  {
    if (m_writeReservation == NoTag) {
      Tag tag;
      if (!m_free.pop(tag)) {
        return nullptr;
      }
      m_writeReservation = tag;
    }
    return &m_slots[m_writeReservation];
  }

  // Publishes the reserved slot to the consumer.
  void commitWrite(T* pmsg)
  {
    ETL_ASSERT(tagOf(pmsg) == m_writeReservation, ETL_ERROR(etl::array_out_of_range));
    m_ready.push(m_writeReservation);
    m_writeReservation = NoTag;
  }

  //--- consumer thread only --------------------------------------------------

  // Returns the oldest published slot, or nullptr if none. Idempotent:
  // returns the same slot until commitRead().
  [[nodiscard]] T* reserveRead()
  {
    if (m_readReservation == NoTag) {
      Tag tag;
      if (!m_ready.pop(tag)) {
        return nullptr;
      }
      m_readReservation = tag;
    }
    return &m_slots[m_readReservation];
  }

  // Returns the slot to circulation. Must be called for every readReserve()
  // that returned non-null, whether or not the message was acted upon.
  void commitRead(T* pmsg)
  {
    ETL_ASSERT(tagOf(pmsg) == m_readReservation, ETL_ERROR(etl::array_out_of_range));
    m_free.push(m_readReservation);
    m_readReservation = NoTag;
  }

  // Published slots not yet reserved by the consumer. Accurate from the
  // consumer thread; a hint only from the producer thread.
  [[nodiscard]] size_t pending() const { return m_ready.size(); }

private:
  Tag tagOf(const T* pmsg) const
  {
    ETL_ASSERT(pmsg >= m_slots.data() && pmsg < m_slots.data() + N,
               ETL_ERROR(etl::array_out_of_range));
    return static_cast<Tag>(pmsg - m_slots.data());
  }

  etl::array<T, N>                   m_slots;
  etl::queue_spsc_atomic<Tag, N>     m_free;               // consumer pushes, producer pops
  etl::queue_spsc_atomic<Tag, N>     m_ready;              // producer pushes, consumer pops
  Tag                                m_writeReservation;   // producer thread only
  Tag                                m_readReservation;    // consumer thread only
};