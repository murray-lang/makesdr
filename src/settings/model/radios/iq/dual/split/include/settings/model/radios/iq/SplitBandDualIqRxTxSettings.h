#pragma once
// #include <settings/model/BandSettingsCacheT.h>
#include <settings/model/proto/RadioSettings.pb.h>
#include <settings/model/proto/RadioPayloads.pb.h>
#include <settings/model/data/radio/RadioLookup.h>
#include <settings/model/radios/RadioSettingsBaseT.h>
#include <settings/model/radios/iq/SplitBandDualIqActiveBandSettings.h>
#include <settings/model/radios/iq/RxTxDualIqBandSettingsCache.h>
#include "SplitBandDualIqRxTxUpdater.h"

using SplitBandDualIqRxTxSettingsBaseType = RadioSettingsBaseT<
    makesdr_SplitBandDualIqRxTxSettingsPb,
    &makesdr_SplitBandDualIqRxTxSettingsPb_msg,
    makesdr_SplitBandDualIqRxTxSettingsPayloadPb,
    makesdr_RadioPayloadType_PAYLOAD_SETTINGS_SPLIT_BAND_DUAL_IQ_RXTX,
    makesdr_SplitBandDualIqRxTxSettingsPayloadPb_size,
    SplitBandDualIqActiveBandSettings,
    RxTxDualIqBandSettingsCache
  >;

class SplitBandDualIqRxTxSettings : public SplitBandDualIqRxTxSettingsBaseType
{
public:
  SplitBandDualIqRxTxSettings();
  SplitBandDualIqRxTxSettings(const Payload& payload, bool complete = true);
  SplitBandDualIqRxTxSettings(const SplitBandDualIqRxTxSettings& other);
  SplitBandDualIqRxTxSettings(SplitBandDualIqRxTxSettings&& other) noexcept;
  SplitBandDualIqRxTxSettings& operator=(const SplitBandDualIqRxTxSettings& other);
  // SplitBandDualIqRxTxSettings& operator=(SplitBandDualIqRxTxSettings&& other) noexcept;

   // [[nodiscard]] bool hasTransmitter() const override { return m_payload.body.has_transmitter; }
  // TransmitterSettings* transmitter() override { return &m_transmitterSettings; }
  // [[nodiscard]] const TransmitterSettings* transmitter() const override { return &m_transmitterSettings; }
  RadioSettingsUpdater* updater() override { return &m_updater; }

  void InitBandAndPipelineIdsWithDefaults();

#ifdef USE_DOTTED_STRING_PATHS
  ResolveDottedStringFunc resolveDottedStringFunc() override;
#endif

protected:
  // TransmitterSettings m_transmitterSettings;
  SplitBandDualIqRxTxUpdater m_updater;
};

