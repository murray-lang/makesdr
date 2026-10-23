#pragma once

#include <ResultCode.h>
#include <settings/model/message/PayloadSource.h>
#include <transport/MessageRingSinkT.h>
#include <settings/model/update/FieldUpdate.h>
#include <settings/model/update/FieldUpdateMessage.h>
#include <settings/model/update/FieldUpdateSink.h>

template<typename RingsT>
class UpdateClientSideRingsT : public FieldUpdateSink
{
public:
  using Rings = RingsT;

  explicit UpdateClientSideRingsT(RingsT& rings)
    : m_updateOut(rings.updateToRadio())
  {
  }

  //--- outgoing (client -> radio) --------------------------------------------
  ResultCode send(FieldUpdateMessage* update)
  {
    update->setSource(PayloadSource::SOURCE_FRONT_END);
    return m_updateOut.applyMessage(update);
  }

  ResultCode applyFieldUpdate(const FieldUpdate& update) override
  {
    FieldUpdateMessage message(update, PayloadSource::SOURCE_FRONT_END);
    return m_updateOut.applyMessage(&message);
  }

private:
  MessageRingSinkT<typename RingsT::UpdateRing>   m_updateOut;
};
