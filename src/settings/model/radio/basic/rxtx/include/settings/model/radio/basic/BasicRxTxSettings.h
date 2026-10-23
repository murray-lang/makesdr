#pragma once
#include <settings/model/radio/BandSettingsCacheT.h>
#include <settings/model/proto/RadioSettings.pb.h>
#include <settings/model/proto/RadioPayloads.pb.h>
#include <settings/model/radio/RadioSettingsBaseT.h>
#include <settings/model/radio/basic/BasicActiveBandSettings.h>
#include <settings/model/radio/basic/BasicBandSettingsCache.h>

using BasicRxTxSettingsBaseType = RadioSettingsBaseT<
    makesdr_BasicRxTxSettingsPb,
    &makesdr_BasicRxTxSettingsPb_msg,
    makesdr_BasicRxTxSettingsPayloadPb,
    makesdr_RadioPayloadType_PAYLOAD_SETTINGS_BASIC_RXTX,
    makesdr_BasicRxTxSettingsPayloadPb_size,
    BasicActiveBandSettings,
    BasicBandSettingsCache
  >;

class BasicRxTxSettings : public BasicRxTxSettingsBaseType
{
public:
  struct Traits
  {
    static constexpr bool hasRx = true;
    static constexpr bool hasTx = true;
    static constexpr bool isDualIq = false;
    static constexpr bool isSplitBand = false;
  };

  BasicRxTxSettings();

  [[nodiscard]] RadioSettingsTraits traits() const { return toRuntimeTraits<Traits>(); }

  [[nodiscard]] bool hasActiveBands() const { return m_payload.body.has_active_bands;}
  ActiveBandSettings& activeBandSettings() { return m_activeBandSettings; }
  [[nodiscard]] const ActiveBandSettings& activeBandSettings() const { return m_activeBandSettings; }

  [[nodiscard]] bool hasTransmitter() const { return m_payload.body.has_transmitter; }
  TransmitterSettings* transmitter() { return &m_transmitterSettings; }
  [[nodiscard]] const TransmitterSettings* transmitter() const { return &m_transmitterSettings; }

#ifdef USE_DOTTED_STRING_PATHS
  ResolveDottedStringFunc resolveDottedStringFunc();
#endif

protected:
  TransmitterSettings m_transmitterSettings;
};
