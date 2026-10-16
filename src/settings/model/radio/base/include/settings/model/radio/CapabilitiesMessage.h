#pragma once

#include <settings/model/message/MessageT.h>
#include "Capabilities.h"

class CapabilitiesMessage : public MessageT<
    makesdr_CapabilitiesPb,
    &makesdr_CapabilitiesPb_msg,
    makesdr_CapabilitiesPayloadPb,
    makesdr_RadioPayloadType_PAYLOAD_CAPABILITIES,
    makesdr_CapabilitiesPayloadPb_size
  >
{
public:
  CapabilitiesMessage()
    : MessageT()
    , m_capabilities(m_payload.body)
  {
  }
  CapabilitiesMessage(const PayloadProto& payload)
    : MessageT(payload)
    , m_capabilities(m_payload.body)
  {

  }
  // CapabilitiesMessage(const Capabilities& field, PayloadSource source)
  // {
  //
  // }
  CapabilitiesMessage(const CapabilitiesMessage& rhs)
    : MessageT(rhs.payload())
    , m_capabilities(m_payload.body)
  {
  }

  CapabilitiesMessage& operator=(const CapabilitiesMessage& rhs)
  {
    if (this != &rhs) {
      m_payload = rhs.m_payload;
    }
    return *this;
  }
protected:
  Capabilities m_capabilities;
};