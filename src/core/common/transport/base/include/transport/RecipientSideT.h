#pragma once

//*****************************************************************************
// One side of a set of rings, notified through a platform-specific RecipientT
// whenever messages addressed to it are waiting.
//
// Inherits the side's whole interface (sinks, sends, publishers, dispatch), so
// nothing need be forwarded. The side owns and attaches the recipient. It
// calls dispatch() in whatever thread or context the platform delivers
// notifications to, which the platform glue arranges through recipient().
//
// Only sides that receive something need this: a side that only sends can be
// used as it is.
//
// Detaching in the destructor, before the recipient is destroyed, makes
// destruction safe wherever the notifier treats detach() as a barrier.
//*****************************************************************************
template<typename SideT, template<typename> class RecipientT>
class RecipientSideT : public SideT
{
public:

  using Rings = typename SideT::Rings;

  template<typename RingsT>
  explicit RecipientSideT(RingsT& rings)
    : SideT(rings)
    , m_recipient(*this)
  {
  }

  ~RecipientSideT() { detachRecipient(); }

  RecipientSideT(const RecipientSideT&) = delete;
  RecipientSideT& operator=(const RecipientSideT&) = delete;

  // Hide the side's versions: the owned recipient is the only one.
  void attachRecipient() { SideT::attachRecipient(&m_recipient); }
  void detachRecipient() { SideT::detachRecipient(); }

  RecipientT<SideT>& recipient() { return m_recipient; }

private:
  RecipientT<SideT> m_recipient;
};
