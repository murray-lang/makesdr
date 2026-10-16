#pragma once
#include <settings/model/radio/BandSettingsCacheT.h>
#include <settings/model/proto/RadioSettings.pb.h>
#include <settings/model/proto/RadioPayloads.pb.h>
#include <settings/model/radio/RadioSettingsBaseT.h>
#include <settings/model/radio/iq/DualIqActiveBandSettings.h>
#include <settings/model/radio/iq/DualIqBandSettingsCache.h>

#ifdef USE_DOTTED_STRING_PATHS
#include <settings/model/update/ResolveDottedStringFunc.h>
#endif

#include "DualIqRxUpdater.h"

using DualIqRxSettingsBaseType = RadioSettingsBaseT<
    makesdr_DualIqRxSettingsPb,
    &makesdr_DualIqRxSettingsPb_msg,
    makesdr_DualIqRxSettingsPayloadPb,
    makesdr_RadioPayloadType_PAYLOAD_SETTINGS_DUAL_IQ_RX,
    makesdr_DualIqRxSettingsPayloadPb_size,
    DualIqActiveBandSettings,
    DualIqBandSettingsCache
  >;

class DualIqRxSettings : public DualIqRxSettingsBaseType
{
public:
  using Proto = makesdr_DualIqRxSettingsPb;
  using Payload = makesdr_DualIqRxSettingsPayloadPb;
  using Cache = DualIqBandSettingsCache;

  DualIqRxSettings();
  DualIqRxSettings(const Payload& payload, bool complete = true);
  DualIqRxSettings(const DualIqRxSettings& other);
  DualIqRxSettings(DualIqRxSettings&& other) noexcept;
  DualIqRxSettings& operator=(const DualIqRxSettings& other);

  RadioSettingsUpdater* updater() override { return &m_updater; }

  void InitBandAndPipelineIdsWithDefaults();

#ifdef USE_DOTTED_STRING_PATHS
  ResolveDottedStringFunc resolveDottedStringFunc() override;
#endif

protected:
  DualIqRxUpdater m_updater;

};