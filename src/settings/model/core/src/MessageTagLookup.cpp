#include "settings/model/core/MessageTagLookup.h"
#include "settings/model/proto/RadioSettings.pb.h"


constexpr FieldEntry mode_fields[] = {
  {"type", RadioSettings_ModePb_type_tag,  nullptr},
  {"name", RadioSettings_ModePb_name_tag,  nullptr},
  {"label", RadioSettings_ModePb_label_tag,  nullptr},
  {"lo_cut", RadioSettings_ModePb_lo_cut_tag,  nullptr},
  {"hi_cut", RadioSettings_ModePb_hi_cut_tag,  nullptr},
  {"offset", RadioSettings_ModePb_offset_tag,  nullptr},
  {nullptr, 0,  nullptr}
};
constexpr FieldEntry band_fields[] = {
  {"name", RadioSettings_BandPb_name_tag,  nullptr},
  {"label", RadioSettings_BandPb_label_tag,  nullptr},
  {"lowest_frequency", RadioSettings_BandPb_lowest_frequency_tag,  nullptr},
  {"highest_frequency", RadioSettings_BandPb_highest_frequency_tag,  nullptr},
  {"landing_frequency", RadioSettings_BandPb_landing_frequency_tag,  nullptr},
  {"default_fine_step", RadioSettings_BandPb_default_fine_step_tag,  nullptr},
  {"default_coarse_step", RadioSettings_BandPb_default_coarse_step_tag,  nullptr},
  {"default_mode", RadioSettings_BandPb_default_mode_tag,  mode_fields},
  {nullptr, 0,  nullptr}
};

// Assume the same tags for all steppable messages. Using tags for int64 version.
constexpr FieldEntry steppable_fields[] = {
    {"value", RadioSettings_SteppableInt64SettingPb_value_tag,  nullptr},
    {"coarse_delta", RadioSettings_SteppableInt64SettingPb_coarse_delta_tag,  nullptr},
    {"fine_delta", RadioSettings_SteppableInt64SettingPb_fine_delta_tag,  nullptr},
    {"use_fine", RadioSettings_SteppableInt64SettingPb_use_fine_tag,  nullptr},
    {nullptr, 0,  nullptr}
};

constexpr FieldEntry iq_corrections_fields[] = {
  {"amplitude", 1001, steppable_fields},
  {"phase", 1002, steppable_fields},
  {nullptr, 0,  nullptr}
};

constexpr FieldEntry rf_fields[] = {
    {"gain", RadioSettings_RfSettingsPb_gain_tag,  steppable_fields},
    {"centre_frequency", RadioSettings_RfSettingsPb_centre_frequency_tag,  steppable_fields},
    {"vfo", RadioSettings_RfSettingsPb_vfo_tag,  steppable_fields},
    {"max_negative_vfo_offset", RadioSettings_RfSettingsPb_max_negative_vfo_offset_tag,  nullptr},
    {"max_positive_vfo_offset", RadioSettings_RfSettingsPb_max_positive_vfo_offset_tag,  nullptr},
    {nullptr, 0,  nullptr}
};

constexpr FieldEntry if_fields[] = {
  {"gain", RadioSettings_IfSettingsPb_gain_tag,  steppable_fields},
  {"bandwidth", RadioSettings_IfSettingsPb_bandwidth_tag,  nullptr},
  {nullptr, 0,  nullptr}
};

constexpr FieldEntry mic_fields[] = {
  {"gain", RadioSettings_MicrophoneSettingsPb_gain_tag,  steppable_fields},
  {nullptr, 0,  nullptr}
};

constexpr FieldEntry receiver_fields[] = {
  {"iq_corrections", RadioSettings_ReceiverSettings_iq_corrections_tag,  iq_corrections_fields},
  {nullptr, 0,  nullptr}
};

constexpr FieldEntry transmitter_fields[] = {
  {"iq_corrections", RadioSettings_TransmitterSettingsPb_iq_corrections_tag,  iq_corrections_fields},
  {"mic", 2,  mic_fields},
  {nullptr, 0,  nullptr}
};

constexpr FieldEntry pipeline_fields[] = {
  {"mode_request", RadioSettings_PipelineSettingsPb_mode_request_tag, nullptr, false, AutoCompleteTrigger::MODE},
  {"mode", RadioSettings_PipelineSettingsPb_mode_tag,  mode_fields},
  {"rf", RadioSettings_PipelineSettingsPb_rf_tag,  rf_fields},
  {nullptr, 0,  nullptr}
};

constexpr FieldEntry rx_pipeline_fields[] = {
  {"base", RadioSettings_RxPipelineSettingsPb_base_tag,  pipeline_fields},
  {"if", RadioSettings_RxPipelineSettingsPb_if_tag,  if_fields},
  {"mute", RadioSettings_RxPipelineSettingsPb_mute_tag,  nullptr},
  {"agc_speed", RadioSettings_RxPipelineSettingsPb_agc_speed_tag,  nullptr},
  {nullptr, 0,  nullptr}
};

constexpr FieldEntry tx_pipeline_fields[] = {
  {"base", RadioSettings_RxPipelineSettingsPb_base_tag,  pipeline_fields},
  {nullptr, 0,  nullptr}
};

constexpr FieldEntry band_settings_fields[] = {
  {"band_request", RadioSettings_BandSettingsPb_band_request_tag, nullptr, false, AutoCompleteTrigger::BAND},
  {"band", RadioSettings_BandSettingsPb_band_tag,  band_fields},
  {"pipeline_a", RadioSettings_BandSettingsPb_pipeline_a_tag,  rx_pipeline_fields},
  {"pipeline_b", RadioSettings_BandSettingsPb_pipeline_b_tag,  rx_pipeline_fields},
  {"tx_pipeline", RadioSettings_BandSettingsPb_tx_pipeline_tag,  tx_pipeline_fields},
  {"is_multi_pipeline", RadioSettings_BandSettingsPb_is_multi_pipeline_tag,  nullptr, false, AutoCompleteTrigger::MULTI_PIPELINE},
  {"focus_pipeline_id", RadioSettings_BandSettingsPb_focus_pipeline_id_tag,  nullptr},
  {"tx_pipeline_id", RadioSettings_BandSettingsPb_tx_pipeline_id_tag,  nullptr},
  {"focus_pipeline", RadioSettings_BandSettingsPb_focus_pipeline_tag,  rx_pipeline_fields, true},
  {nullptr, 0,  nullptr}
};

constexpr FieldEntry active_bands_fields[] = {
  {"focus_band_id", RadioSettings_ActiveBandSettingsPb_focus_band_id_tag,  nullptr},
  {"tx_band_id", RadioSettings_ActiveBandSettingsPb_tx_band_id_tag,  nullptr},
  {"rx_band_id", RadioSettings_ActiveBandSettingsPb_rx_band_id_tag,  nullptr},
  {"band_1", RadioSettings_ActiveBandSettingsPb_band_1_tag,  band_settings_fields},
  {"band_2", RadioSettings_ActiveBandSettingsPb_band_2_tag,  band_settings_fields},
  {"is_split", RadioSettings_ActiveBandSettingsPb_is_split_tag, nullptr, false, AutoCompleteTrigger::SPLIT_BAND },
  {"focus_band", RadioSettings_ActiveBandSettingsPb_focus_band_tag,  band_settings_fields, true},
  {nullptr, 0,  nullptr}
};

const FieldEntry radio_fields[] = {
  {"receiver", RadioSettings_RadioSettingsPb_receiver_tag,  receiver_fields},
  {"transmitter", RadioSettings_RadioSettingsPb_transmitter_tag,  transmitter_fields},
  {"active_bands", RadioSettings_RadioSettingsPb_active_bands_tag,  active_bands_fields},
  {"ptt", RadioSettings_RadioSettingsPb_ptt_tag,  nullptr},
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
