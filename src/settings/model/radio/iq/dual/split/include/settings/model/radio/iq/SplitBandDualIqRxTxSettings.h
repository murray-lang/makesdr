#pragma once
// #include <settings/model/BandSettingsCacheT.h>
#include <settings/model/proto/RadioSettings.pb.h>
#include <settings/model/proto/RadioPayloads.pb.h>
#include <settings/model/radio/RadioSettingsBaseT.h>
#include <settings/model/radio/iq/SplitBandDualIqActiveBandSettings.h>
#include <settings/model/radio/iq/RxTxDualIqBandSettingsCache.h>
#include "SplitBandDualIqRxTxUpdater.h"

#ifdef USE_DOTTED_STRING_PATHS
#include <settings/model/update/ResolveDottedStringFunc.h>
#endif

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
  using ActiveBandSettings = SplitBandDualIqActiveBandSettings;

  struct Traits
  {
    static constexpr bool hasRx = true;
    static constexpr bool hasTx = true;
    static constexpr bool isDualIq = true;
    static constexpr bool isSplitBand = true;
  };

  [[nodiscard]] RadioSettingsTraits traits() const { return toRuntimeTraits<Traits>(); }

  SplitBandDualIqRxTxSettings();
  SplitBandDualIqRxTxSettings(const Payload& payload, bool complete = true);
  SplitBandDualIqRxTxSettings(const SplitBandDualIqRxTxSettings& other);
  SplitBandDualIqRxTxSettings(SplitBandDualIqRxTxSettings&& other) noexcept;
  SplitBandDualIqRxTxSettings& operator=(const SplitBandDualIqRxTxSettings& other);
  // SplitBandDualIqRxTxSettings& operator=(SplitBandDualIqRxTxSettings&& other) noexcept;

   // [[nodiscard]] bool hasTransmitter() const { return m_payload.body.has_transmitter; }
  // TransmitterSettings* transmitter() { return &m_transmitterSettings; }
  // [[nodiscard]] const TransmitterSettings* transmitter() const { return &m_transmitterSettings; }
  RadioSettingsUpdater* updater() { return &m_updater; }

  void InitBandAndPipelineIdsWithDefaults();

#ifdef USE_DOTTED_STRING_PATHS
  ResolveDottedStringFunc resolveDottedStringFunc();
#endif

protected:
  // TransmitterSettings m_transmitterSettings;
  SplitBandDualIqRxTxUpdater m_updater;
};

