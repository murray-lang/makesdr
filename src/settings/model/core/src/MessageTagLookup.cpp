#include "settings/model/core/MessageTagLookup.h"
#include "settings/model/proto/RadioSettings.pb.h"


constexpr FieldEntry mode_fields[] = {
  {"type", makesdr_ModePb_type_tag,  nullptr},
  {"name", makesdr_ModePb_name_tag,  nullptr},
  {"label", makesdr_ModePb_label_tag,  nullptr},
  {"lo_cut", makesdr_ModePb_lo_cut_tag,  nullptr},
  {"hi_cut", makesdr_ModePb_hi_cut_tag,  nullptr},
  {"offset", makesdr_ModePb_offset_tag,  nullptr},
  {nullptr, 0,  nullptr}
};
constexpr FieldEntry band_fields[] = {
  {"name", makesdr_BandPb_name_tag,  nullptr},
  {"label", makesdr_BandPb_label_tag,  nullptr},
  {"lowest_frequency", makesdr_BandPb_lowest_frequency_tag,  nullptr},
  {"highest_frequency", makesdr_BandPb_highest_frequency_tag,  nullptr},
  {"landing_frequency", makesdr_BandPb_landing_frequency_tag,  nullptr},
  {"default_fine_step", makesdr_BandPb_default_fine_step_tag,  nullptr},
  {"default_coarse_step", makesdr_BandPb_default_coarse_step_tag,  nullptr},
  {"default_mode", makesdr_BandPb_default_mode_tag,  mode_fields},
  {nullptr, 0,  nullptr}
};

// Assume the same tags for all steppable messages. Using tags for int64 version.
constexpr FieldEntry steppable_fields[] = {
    {"value", makesdr_SteppableInt64SettingPb_value_tag,  nullptr},
    {"coarse_delta", makesdr_SteppableInt64SettingPb_coarse_delta_tag,  nullptr},
    {"fine_delta", makesdr_SteppableInt64SettingPb_fine_delta_tag,  nullptr},
    {"use_fine", makesdr_SteppableInt64SettingPb_use_fine_tag,  nullptr},
    {nullptr, 0,  nullptr}
};

constexpr FieldEntry iq_corrections_fields[] = {
  {"amplitude", 1001, steppable_fields},
  {"phase", 1002, steppable_fields},
  {nullptr, 0,  nullptr}
};

constexpr FieldEntry rf_fields[] = {
    {"gain", makesdr_RfSettingsPb_gain_tag,  steppable_fields},
    {"centre_frequency", makesdr_RfSettingsPb_centre_frequency_tag,  steppable_fields},
    {"vfo", makesdr_RfSettingsPb_vfo_tag,  steppable_fields},
    {"max_negative_vfo_offset", makesdr_RfSettingsPb_max_negative_vfo_offset_tag,  nullptr},
    {"max_positive_vfo_offset", makesdr_RfSettingsPb_max_positive_vfo_offset_tag,  nullptr},
    {nullptr, 0,  nullptr}
};

constexpr FieldEntry if_fields[] = {
  {"gain", makesdr_IfSettingsPb_gain_tag,  steppable_fields},
  {"bandwidth", makesdr_IfSettingsPb_bandwidth_tag,  nullptr},
  {nullptr, 0,  nullptr}
};

constexpr FieldEntry mic_fields[] = {
  {"gain", makesdr_MicrophoneSettingsPb_gain_tag,  steppable_fields},
  {nullptr, 0,  nullptr}
};

constexpr FieldEntry receiver_fields[] = {
  {"iq_corrections", makesdr_ReceiverSettings_iq_corrections_tag,  iq_corrections_fields},
  {nullptr, 0,  nullptr}
};

constexpr FieldEntry transmitter_fields[] = {
  {"iq_corrections", makesdr_TransmitterSettingsPb_iq_corrections_tag,  iq_corrections_fields},
  {"mic", 2,  mic_fields},
  {nullptr, 0,  nullptr}
};

constexpr FieldEntry pipeline_fields[] = {
  {"mode_request", makesdr_PipelineSettingsPb_mode_request_tag, nullptr, false, AutoCompleteTrigger::MODE},
  {"mode", makesdr_PipelineSettingsPb_mode_tag,  mode_fields},
  {"rf", makesdr_PipelineSettingsPb_rf_tag,  rf_fields},
  {nullptr, 0,  nullptr}
};

constexpr FieldEntry rx_pipeline_fields[] = {
  {"base", makesdr_RxPipelineSettingsPb_base_tag,  pipeline_fields},
  {"if", makesdr_RxPipelineSettingsPb_if_tag,  if_fields},
  {"mute", makesdr_RxPipelineSettingsPb_mute_tag,  nullptr},
  {"agc_speed", makesdr_RxPipelineSettingsPb_agc_speed_tag,  nullptr},
  {nullptr, 0,  nullptr}
};

constexpr FieldEntry tx_pipeline_fields[] = {
  {"base", makesdr_RxPipelineSettingsPb_base_tag,  pipeline_fields},
  {nullptr, 0,  nullptr}
};

constexpr FieldEntry band_settings_fields[] = {
  {"band_request", makesdr_BandSettingsPb_band_request_tag, nullptr, false, AutoCompleteTrigger::BAND},
  {"band", makesdr_BandSettingsPb_band_tag,  band_fields},
  {"pipeline_a", makesdr_BandSettingsPb_pipeline_a_tag,  rx_pipeline_fields},
  {"pipeline_b", makesdr_BandSettingsPb_pipeline_b_tag,  rx_pipeline_fields},
  {"tx_pipeline", makesdr_BandSettingsPb_tx_pipeline_tag,  tx_pipeline_fields},
  {"is_multi_pipeline", makesdr_BandSettingsPb_is_multi_pipeline_tag,  nullptr, false, AutoCompleteTrigger::MULTI_PIPELINE},
  {"focus_pipeline_id", makesdr_BandSettingsPb_focus_pipeline_id_tag,  nullptr},
  {"tx_pipeline_id", makesdr_BandSettingsPb_tx_pipeline_id_tag,  nullptr},
  {"focus_pipeline", makesdr_BandSettingsPb_focus_pipeline_tag,  rx_pipeline_fields, true},
  {nullptr, 0,  nullptr}
};

constexpr FieldEntry active_bands_fields[] = {
  {"focus_band_id", makesdr_ActiveBandSettingsPb_focus_band_id_tag,  nullptr},
  {"tx_band_id", makesdr_ActiveBandSettingsPb_tx_band_id_tag,  nullptr},
  {"rx_band_id", makesdr_ActiveBandSettingsPb_rx_band_id_tag,  nullptr},
  {"band_1", makesdr_ActiveBandSettingsPb_band_1_tag,  band_settings_fields},
  {"band_2", makesdr_ActiveBandSettingsPb_band_2_tag,  band_settings_fields},
  {"is_split", makesdr_ActiveBandSettingsPb_is_split_tag, nullptr, false, AutoCompleteTrigger::SPLIT_BAND },
  {"focus_band", makesdr_ActiveBandSettingsPb_focus_band_tag,  band_settings_fields, true},
  {nullptr, 0,  nullptr}
};

const FieldEntry radio_fields[] = {
  {"receiver", makesdr_RadioSettingsPb_receiver_tag,  receiver_fields},
  {"transmitter", makesdr_RadioSettingsPb_transmitter_tag,  transmitter_fields},
  {"active_bands", makesdr_RadioSettingsPb_active_bands_tag,  active_bands_fields},
  {"ptt", makesdr_RadioSettingsPb_ptt_tag,  nullptr},
  {nullptr, 0,  nullptr}  // Sentinel
};

// Lookup function that traverses the hierarchy
// ResultCode lookup_field_path(
//   const char* path,
//   uint32_t* tags_out,
//   uint32_t max_tags,
//   bool* is_indirect_out,
//   AutoCompleteTrigger* trigger_out
// )
// {
//   const FieldEntry* current_table = radio_fields;
//   int tag_count = 0;
//
//   const char* p = path;
//   char field_name[MAX_FIELD_NAME_LENGTH];
//   bool is_indirect = false;
//
//   while (*p && tag_count < max_tags) {
//     // Extract next field name
//     int i = 0;
//     while (*p && *p != '.' && i < 6) {
//       field_name[i++] = *p++;
//     }
//     field_name[i] = '\0';
//     if (*p == '.') p++; // Skip dot
//
//     // Search in current table
//     const FieldEntry* entry = current_table;
//     bool found = false;
//     while (entry != nullptr && entry->name != nullptr) {
//       if (strcmp(entry->name, field_name) == 0) {
//         tags_out[tag_count++] = entry->tag;
//         if (entry->isIndirect) {
//           is_indirect = true;
//         }
//         current_table = entry->submsg;
//         found = true;
//         break;
//       }
//       entry++;
//     }
//
//     if (!found) {
//       return ResultCode::ERR_SETTING_PATH_NOT_FOUND;
//     }
//     if (current_table == nullptr) {
//       if (*p != '\0') {
//         return ResultCode::ERR_SETTING_DOTTED_STRING_NOT_VALID;
//       }
//       *trigger_out = entry->trigger;
//     }
//   }
//   *is_indirect_out = is_indirect;
//   return tag_count > 0 ? ResultCode::OK : ResultCode::ERR_SETTING_PATH_NOT_FOUND;
// }
