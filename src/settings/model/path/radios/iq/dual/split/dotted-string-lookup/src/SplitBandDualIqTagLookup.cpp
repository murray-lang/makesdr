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

const FieldEntry transmitter_fields[] = {
  {"mic", makesdr_TransmitterSettingsPb_mic_tag,  af_fields},
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

const FieldEntry tx_pipeline_fields[] = {
  {"base", makesdr_TxPipelineSettingsPb_base_tag,  pipeline_fields},
  {nullptr, 0,  nullptr}
};

const FieldEntry band_settings_fields[] = {
  {"band_request", makesdr_RxTxDualIqBandSettingsPb_band_request_tag, nullptr, false, true},
  {"band", makesdr_RxTxDualIqBandSettingsPb_band_tag,  band_fields},
  {"rf", makesdr_RxTxDualIqBandSettingsPb_rf_tag,  band_rf_fields},
  {"if", makesdr_RxTxDualIqBandSettingsPb_if_tag,  if_fields},
  {"focus_pipeline", makesdr_RxTxDualIqBandSettingsPb_focus_pipeline_tag,  rx_pipeline_fields, true},
  {"pipeline_a", makesdr_RxTxDualIqBandSettingsPb_pipeline_a_tag,  rx_pipeline_fields},
  {"pipeline_b", makesdr_RxTxDualIqBandSettingsPb_pipeline_b_tag,  rx_pipeline_fields},
  {"is_multi_pipeline", makesdr_RxTxDualIqBandSettingsPb_is_multi_pipeline_tag,  nullptr, false, true},
  {"focus_pipeline_id", makesdr_RxTxDualIqBandSettingsPb_focus_pipeline_id_tag,  nullptr},
  {"tx_pipeline", makesdr_RxTxDualIqBandSettingsPb_tx_pipeline_tag,  tx_pipeline_fields},
  {"tx_pipeline_id", makesdr_RxTxDualIqBandSettingsPb_tx_pipeline_id_tag,  nullptr},
  {nullptr, 0,  nullptr}
};

const FieldEntry active_bands_fields[] = {
  {"focus_band", makesdr_SplitBandDualIqActiveBandSettingsPb_focus_band_tag,  band_settings_fields, true},
  {"band_1", makesdr_SplitBandDualIqActiveBandSettingsPb_band_1_tag,  band_settings_fields},
  {"band_2", makesdr_SplitBandDualIqActiveBandSettingsPb_band_2_tag,  band_settings_fields},
  {"focus_band_id", makesdr_SplitBandDualIqActiveBandSettingsPb_focus_band_id_tag,  nullptr},
  {"tx_band_id", makesdr_SplitBandDualIqActiveBandSettingsPb_tx_band_id_tag,  nullptr},
  {"rx_band_id", makesdr_SplitBandDualIqActiveBandSettingsPb_rx_band_id_tag,  nullptr},
  {"is_split", makesdr_SplitBandDualIqActiveBandSettingsPb_is_split_tag, nullptr, false, true },

  {nullptr, 0,  nullptr}
};

extern const FieldEntry split_band_dual_iq_radio_fields[] = {
  {"active_bands", makesdr_SplitBandDualIqRxTxSettingsPb_active_bands_tag,  active_bands_fields},
  {"receiver", makesdr_SplitBandDualIqRxTxSettingsPb_receiver_tag,  receiver_fields},
  {"transmitter", makesdr_SplitBandDualIqRxTxSettingsPb_transmitter_tag,  transmitter_fields},
  {"ptt", makesdr_SplitBandDualIqRxTxSettingsPb_ptt_tag,  nullptr},
  {nullptr, 0,  nullptr}  // Sentinel
};
