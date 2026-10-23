#pragma once
#include <settings/model/radio/BandSettingsCacheT.h>
#include <settings/model/proto/RadioSettings.pb.h>
#include <settings/model/proto/RadioPayloads.pb.h>
#include <settings/model/radio/RadioSettingsBaseT.h>
#include <settings/model/radio/iq/SingleIqActiveBandSettings.h>

using SingleIqRxTxSettingsBaseType = RadioSettingsBaseT<
    makesdr_SingleIqRxTxSettingsPb,
    &makesdr_SingleIqRxTxSettingsPb_msg,
    makesdr_SingleIqRxTxSettingsPayloadPb,
    makesdr_RadioPayloadType_PAYLOAD_SETTINGS_SINGLE_IQ_RXTX,
    makesdr_SingleIqRxTxSettingsPayloadPb_size,
    SingleIqActiveBandSettings,
    SingleIqBandSettingsCache
  >;

class SingleIqRxTxSettings : public SingleIqRxTxSettingsBaseType
{
public:

  using ActiveBandSettings = SingleIqActiveBandSettings;

  struct Traits
  {
    static constexpr bool hasRx = true;
    static constexpr bool hasTx = true;
    static constexpr bool isDualIq = false;
    static constexpr bool isSplitBand = false;
  };

  SingleIqRxTxSettings();

  [[nodiscard]] RadioSettingsTraits traits() const { return toRuntimeTraits<Traits>(); }

  [[nodiscard]] bool hasTransmitter() const { return m_payload.body.has_transmitter; }
  TransmitterSettings* transmitter() { return &m_transmitterSettings; }
  [[nodiscard]] const TransmitterSettings* transmitter() const { return &m_transmitterSettings; }

#ifdef USE_DOTTED_STRING_PATHS
  ResolveDottedStringFunc resolveDottedStringFunc();
#endif

protected:
  TransmitterSettings m_transmitterSettings;
};

