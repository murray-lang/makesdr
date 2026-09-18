#pragma once
#include <settings/model/radios/BandSettingsCacheT.h>
#include <settings/model/proto/RadioSettings.pb.h>

using RxTxDualIqBandSettingsCache = BandSettingsCacheT<
  makesdr_RxTxDualIqBandSettingsCachePb,
  &makesdr_RxTxDualIqBandSettingsCachePb_msg,
  makesdr_RadioCacheRxTxDualIqPayloadPb,
  makesdr_RadioPayloadType_PAYLOAD_CACHE_DUAL_IQ_RXTX,
  makesdr_RadioCacheRxTxDualIqPayloadPb_size,
  makesdr_RxTxDualIqBandSettingsPb,
  makesdr_RxTxDualIqBandSettingsCachePb_BandSettingsEntry
>;
