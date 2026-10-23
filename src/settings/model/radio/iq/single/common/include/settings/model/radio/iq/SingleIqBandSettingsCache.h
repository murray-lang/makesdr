#pragma once
#include <settings/model/radio/BandSettingsCacheT.h>
#include <settings/model/proto/RadioPayloads.pb.h>
#include <settings/model/proto/RadioSettings.pb.h>

using SingleIqBandSettingsCache = BandSettingsCacheT<
  makesdr_SingleIqBandSettingsCachePb,
  &makesdr_SingleIqBandSettingsCachePb_msg,
  makesdr_RadioCacheSingleIqPayloadPb,
  makesdr_RadioPayloadType_PAYLOAD_CACHE_SINGLE_IQ,
  makesdr_RadioCacheSingleIqPayloadPb_size,
  makesdr_SingleIqBandSettingsPb,
  makesdr_SingleIqBandSettingsCachePb_BandSettingsEntry
>;
