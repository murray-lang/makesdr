#pragma once
#include <settings/model/data/band/BandCategoryList.h>
#include <settings/model/data/mode/ModeList.h>
#include <settings/model/message/MessageT.h>
#include <settings/model/proto/RadioPayloads.pb.h>
#include <settings/model/proto/RadioSettings.pb.h>

using RadioLookupMessage = MessageT<
    makesdr_RadioLookupPb,
    &makesdr_RadioLookupPb_msg,
    makesdr_RadioLookupPayloadPb,
    makesdr_RadioPayloadType_PAYLOAD_LOOKUP,
    makesdr_RadioLookupPayloadPb_size
  >;

class RadioLookup : public RadioLookupMessage
{
public:

  RadioLookup()
    : RadioLookupMessage()
    , m_bands(m_payload.body.bands)
    , m_modes(m_payload.body.modes)
  {}

  RadioLookup(PayloadProto& payload)
    : RadioLookupMessage(payload)
    , m_bands(payload.body.bands)
    , m_modes(payload.body.modes)
  {}

  RadioLookup(RadioLookup& rhs) : RadioLookup(rhs.m_payload) {}

  RadioLookup(RadioLookup&&) = default;

  [[nodiscard]] bool hasBands() const { return m_payload.body.has_bands; }
  [[nodiscard]] bool hasModes() const { return m_payload.body.has_modes; }

  BandCategoryList& bands() { return m_bands; }
  [[nodiscard]] const BandCategoryList& bands() const { return m_bands; }
  ModeList& modes() { return m_modes; }
  [[nodiscard]] const ModeList& modes() const { return m_modes; }

  [[nodiscard]] const makesdr_ModePb* getModeByType(makesdr_ModeType type) const
  {
    return m_modes.findModeByType(type);
  }

protected:
  BandCategoryList m_bands;
  ModeList m_modes;
};
