#pragma once
#include <settings/model/radio/BandSettingsCacheT.h>
#include <settings/model/proto/RadioSettings.pb.h>
#include <settings/model/proto/RadioPayloads.pb.h>
#include <settings/model/radio/RadioSettingsBaseT.h>
#include <settings/model/radio/iq/RxTxDualIqActiveBandSettings.h>
#include <settings/model/radio/iq/RxTxDualIqBandSettingsCache.h>

#ifdef USE_DOTTED_STRING_PATHS
#include <settings/model/update/ResolveDottedStringFunc.h>
#endif
#include "DualIqRxTxUpdater.h"


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
  DualIqRxTxSettings(const Payload& payload, bool complete = true);
  DualIqRxTxSettings(const DualIqRxTxSettings& other);
  DualIqRxTxSettings(DualIqRxTxSettings&& other) noexcept;
  DualIqRxTxSettings& operator=(const DualIqRxTxSettings& other);
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
  DualIqRxTxUpdater m_updater;
};

