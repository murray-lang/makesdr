#pragma once
#include <settings/model/radio/BandSettingsCacheT.h>
#include <settings/model/proto/RadioSettings.pb.h>
#include <settings/model/proto/RadioPayloads.pb.h>
#include <settings/model/meta/radio/RadioLookup.h>
#include <settings/model/radio/RadioSettingsBaseT.h>
#include <settings/model/radio/iq/RxTxDualIqActiveBandSettings.h>
#include <settings/model/radio/iq/RxTxDualIqBandSettingsCache.h>


using DualIqRxTxSettingsBaseType = RadioSettingsBaseT<
    makesdr_DualIqRxTxSettingsPb,
    &makesdr_DualIqRxTxSettingsPb_msg,
    makesdr_DualIqRxTxSettingsPayloadPb,
    makesdr_RadioPayloadType_PAYLOAD_SETTINGS_DUAL_IQ_RXTX,
    makesdr_DualIqRxTxSettingsPayloadPb_size,
    RxTxDualIqActiveBandSettings,
    RxTxDualIqBandSettingsCache
  >;

class DualIqRxTxSettings : public DualIqRxTxSettingsBaseType
{
public:

  using Proto = makesdr_DualIqRxTxSettingsPb;
  using Payload = makesdr_DualIqRxTxSettingsPayloadPb;
  using Cache = RxTxDualIqBandSettingsCache;
  DualIqRxTxSettings();

  [[nodiscard]] bool hasTransmitter() const override { return m_payload.body.has_transmitter; }
  TransmitterSettings* transmitter() override { return &m_transmitterSettings; }
  [[nodiscard]] const TransmitterSettings* transmitter() const override { return &m_transmitterSettings; }

#ifdef USE_DOTTED_STRING_PATHS
  ResolveDottedStringFunc resolveDottedStringFunc() override;
#endif

protected:
  TransmitterSettings m_transmitterSettings;
};

