#pragma once
#include <settings/model/radios/BandSettingsCacheT.h>
#include <settings/model/proto/RadioSettings.pb.h>
#include <settings/model/proto/RadioPayloads.pb.h>
#include <settings/model/data/radio/RadioLookup.h>
#include <settings/model/radios/RadioSettingsBaseT.h>
#include <settings/model/radios/iq/BasicIqActiveBandSettings.h>

using BasicIqRxTxSettingsBaseType = RadioSettingsBaseT<
    makesdr_BasicIqRxTxSettingsPb,
    &makesdr_BasicIqRxTxSettingsPb_msg,
    makesdr_BasicIqRxTxSettingsPayloadPb,
    makesdr_RadioPayloadType_PAYLOAD_SETTINGS_BASIC_IQ_RXTX,
    makesdr_BasicIqRxTxSettingsPayloadPb_size,
    BasicIqActiveBandSettings,
    BasicIqBandSettingsCache
  >;

class BasicIqRxTxSettings : public BasicIqRxTxSettingsBaseType
{
public:
  BasicIqRxTxSettings();

  [[nodiscard]] bool hasTransmitter() const override { return m_payload.body.has_transmitter; }
  TransmitterSettings* transmitter() override { return &m_transmitterSettings; }
  [[nodiscard]] const TransmitterSettings* transmitter() const override { return &m_transmitterSettings; }

#ifdef USE_DOTTED_STRING_PATHS
  ResolveDottedStringFunc resolveDottedStringFunc() override;
#endif

protected:
  TransmitterSettings m_transmitterSettings;
};

