#pragma once
#include <settings/model/radios/BandSettingsCacheT.h>
#include <settings/model/proto/RadioSettings.pb.h>
#include <settings/model/proto/RadioPayloads.pb.h>
#include <settings/model/data/radio/RadioLookup.h>
#include <settings/model/radios/RadioSettingsBaseT.h>
#include <settings/model/radios/basic/BasicActiveBandSettings.h>
#include <settings/model/radios/basic/BasicBandSettingsCache.h>

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
  BasicRxTxSettings();

  [[nodiscard]] bool hasActiveBands() const override { return m_payload.body.has_active_bands;}
  IActiveBandSettings& activeBandSettings() { return m_activeBandSettings; }
  [[nodiscard]] const IActiveBandSettings& activeBandSettings() const { return m_activeBandSettings; }

  [[nodiscard]] bool hasTransmitter() const override { return m_payload.body.has_transmitter; }
  TransmitterSettings* transmitter() override { return &m_transmitterSettings; }
  [[nodiscard]] const TransmitterSettings* transmitter() const override { return &m_transmitterSettings; }

#ifdef USE_DOTTED_STRING_PATHS
  ResolveDottedStringFunc resolveDottedStringFunc() override;
#endif

protected:
  TransmitterSettings m_transmitterSettings;
};
