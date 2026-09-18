#include "settings/model/path/BasicRxTagLookup.h"
#include <settings/model/path/CommonTagLookup.h>
#include <settings/model/proto/RadioSettings.pb.h>



const FieldEntry band_settings_fields[] = {
  {"band_request", makesdr_BasicBandSettingsPb_band_request_tag, nullptr, false, true},
  {"band", makesdr_BasicBandSettingsPb_band_tag,  band_fields},
  {"mode_request", makesdr_BasicBandSettingsPb_mode_request_tag, nullptr, false, true},
  {"mode", makesdr_BasicBandSettingsPb_mode_tag,  mode_fields},
  {"rf", makesdr_BasicBandSettingsPb_rf_tag,  band_rf_fields},
  {"if", makesdr_BasicBandSettingsPb_if_tag,  if_fields},
  {"agc_speed", makesdr_RxPipelineSettingsPb_agc_speed_tag,  nullptr},
  {nullptr, 0,  nullptr}
};

const FieldEntry active_bands_fields[] = {
  {"focus_band", makesdr_BasicActiveBandSettingsPb_focus_band_tag,  band_settings_fields},
  {nullptr, 0,  nullptr}
};

extern const FieldEntry basic_rx_radio_fields[] = {
  {"active_bands", makesdr_BasicRxSettingsPb_active_bands_tag,  active_bands_fields},
  {"receiver", makesdr_BasicRxSettingsPb_receiver_tag,  receiver_fields},
  {"ptt", makesdr_BasicRxSettingsPb_ptt_tag,  nullptr},
  {nullptr, 0,  nullptr}  // Sentinel
};
