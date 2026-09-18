#pragma once
#include <CrossPlatformTypes.h>

#include <ResultCode.h>
#include <settings/model/proto/RadioSettings.pb.h>
#include <settings/model/message/MessageT.h>

#include <settings/model/radios/Mode.h>

#ifdef USE_ETL
#include <etl/span.h>


using ModeSpan = etl::span<const makesdr_ModePb, MAX_MODES>;
#else
#include <span>
using ModeSpan = std::span<const Mode::Proto>;
#endif

using ModeListMessage = MessageT<
  makesdr_ModeListPb,
  &makesdr_ModeListPb_msg,
  makesdr_ModesPayloadPb,
  makesdr_RadioPayloadType_PAYLOAD_MODES,
  makesdr_ModesPayloadPb_size
>;

class ModeList : public ModeListMessage
{
public:
  ModeList()
    : ModeListMessage()
    , m_modes(m_payload.body.modes, m_payload.body.modes_count)
  {
  }

  ModeList(const PayloadProto& payload)
    : ModeListMessage(payload)
    , m_modes(payload.body.modes, payload.body.modes_count)
  {}

  ModeList(const Proto& modes) // TODO: Remove. Broken. Also Remove RadioLookup. Just keeping compiler happy.
    : ModeListMessage()
    , m_modes(m_payload.body.modes, modes.modes_count)
  {
    m_payload.header.payloadType = makesdr_RadioPayloadType_PAYLOAD_MODES;
    m_payload.header.purpose = makesdr_RadioPayloadPurpose_PURPOSE_NONE;
    m_payload.body = modes;
    m_payload.has_header = true;
    m_payload.has_body = true;
  }

  ModeList(const ModeList& other)
    : ModeListMessage(other.m_payload)
    , m_modes(m_payload.body.modes, m_payload.body.modes_count)
  {}

  ModeList(ModeList&& other)  noexcept
  : ModeListMessage(other.m_payload)
  , m_modes(m_payload.body.modes, m_payload.body.modes_count)
  {
  }

  ModeList& operator=(const ModeList& other)
  {
    if (this != &other) {
      m_payload = other.m_payload;
      m_modes = ModeSpan(m_payload.body.modes, m_payload.body.modes_count);
    }

    return *this;
  }

  [[nodiscard]] const ModeSpan& modes() const { return m_modes; }

  [[nodiscard]] const Mode::Proto* findModeByType(makesdr_ModeType type) const
  {
    for (const auto& mode : m_modes) {
      if (mode.type == type) {
        return &mode;
      }
    }
    return nullptr;
  }

protected:
  ModeSpan m_modes;
};