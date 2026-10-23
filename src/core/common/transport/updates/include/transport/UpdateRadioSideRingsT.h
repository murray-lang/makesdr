#pragma once

#include <settings/model/message/PayloadType.h>
#include <settings/model/update/FieldUpdateMessage.h>
#include <transport/MessageRingSourceT.h>

template<typename RingsT>
class UpdateRadioSideRingsT
{
public:
  using Rings = RingsT;

  explicit UpdateRadioSideRingsT(RingsT& rings)
    : m_updateIn(rings.updateToRadio())
  {
  }
  //--- outgoing (radio -> client) --------------------------------------------

  //--- incoming (client -> radio) --------------------------------------------
  void connectFieldUpdateSink(MessageSinkT<FieldUpdateMessage>* sink)  { m_updateIn.connectMessageSink(sink); }

  // Radio thread only. Delivers whatever is pending on the channel the
  // payload type identifies; unknown types are ignored.
  void dispatch(PayloadType payloadType)
  {
    if (payloadType == PAYLOAD_TYPE_FIELD_UPDATE) {
      m_updateIn.consumePending();
    }
  }

  template<typename TargetT>
  void attachRecipient(TargetT target)
  {
    m_updateIn.attachRecipient(target);
  }

  void detachRecipient()
  {
    m_updateIn.detachRecipient();
  }

protected:
  MessageRingSourceT<typename RingsT::UpdateRing>   m_updateIn;
};
