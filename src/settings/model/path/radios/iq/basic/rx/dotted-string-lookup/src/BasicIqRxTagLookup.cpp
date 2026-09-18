#include <settings/model/path/CommonTagLookup.h>
#include <settings/model/proto/RadioSettings.pb.h>


const FieldEntry iq_corrections_fields[] = {
  {"amplitude", 1001, steppable_fields},
  {"phase", 1002, steppable_fields},
  {nullptr, 0,  nullptr}
};

const FieldEntry pipeline_rf_fields[] = {
  {"frequency", makesdr_PipelineRfSettingsPb_frequency_tag,  steppable_fields},
  {"max_negative_offset", makesdr_PipelineRfSettingsPb_max_negative_offset_tag,  nullptr},
  {"max_positive_offset", makesdr_PipelineRfSettingsPb_max_positive_offset_tag,  nullptr},
  {nullptr, 0,  nullptr}
};

const FieldEntry pipeline_fields[] = {
  {"mode_request", makesdr_PipelineSettingsPb_mode_request_tag, nullptr, false, true},
  {"mode", makesdr_PipelineSettingsPb_mode_tag,  mode_fields},
  {"rf", makesdr_PipelineSettingsPb_rf_tag,  pipeline_rf_fields},
  {"iq_corrections", makesdr_PipelineSettingsPb_iq_corrections_tag,  iq_corrections_fields},
  {nullptr, 0,  nullptr}
};

const FieldEntry rx_pipeline_fields[] = {
  {"base", makesdr_RxPipelineSettingsPb_base_tag,  pipeline_fields},
  {"mute", makesdr_RxPipelineSettingsPb_mute_tag,  nullptr},
  {"agc_speed", makesdr_RxPipelineSettingsPb_agc_speed_tag,  nullptr},
  {nullptr, 0,  nullptr}
};

const FieldEntry band_settings_fields[] = {
  {"band_request", makesdr_BasicIqBandSettingsPb_band_request_tag, nullptr, false, true},
  {"band", makesdr_BasicIqBandSettingsPb_band_tag,  band_fields},
  {"rf", makesdr_BasicIqBandSettingsPb_rf_tag,  band_rf_fields},
  {"if", makesdr_BasicIqBandSettingsPb_if_tag,  if_fields},
  {"focus_pipeline", makesdr_BasicIqBandSettingsPb_focus_pipeline_tag,  rx_pipeline_fields},
  {nullptr, 0,  nullptr}
};

const FieldEntry active_bands_fields[] = {
  {"focus_band", makesdr_BasicIqActiveBandSettingsPb_focus_band_tag,  band_settings_fields},
  {nullptr, 0,  nullptr}
};

extern const FieldEntry basic_iq_rx_radio_fields[] = {
  {"active_bands", makesdr_BasicIqRxSettingsPb_active_bands_tag,  active_bands_fields},
  {"receiver", makesdr_BasicIqRxSettingsPb_receiver_tag,  receiver_fields},
  {"ptt", makesdr_BasicIqRxSettingsPb_ptt_tag,  nullptr},
  {nullptr, 0,  nullptr}  // Sentinel
};
