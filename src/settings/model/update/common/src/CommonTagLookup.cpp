#include "settings/model/update/CommonTagLookup.h"
#include <settings/model/proto/RadioSettings.pb.h>


extern const FieldEntry mode_fields[] = {
  {"type", makesdr_ModePb_type_tag,  nullptr},
  {"name", makesdr_ModePb_name_tag,  nullptr},
  {"label", makesdr_ModePb_label_tag,  nullptr},
  {"lo_cut", makesdr_ModePb_lo_cut_tag,  nullptr},
  {"hi_cut", makesdr_ModePb_hi_cut_tag,  nullptr},
  {"offset", makesdr_ModePb_offset_tag,  nullptr},
  {nullptr, 0,  nullptr}
};
extern const FieldEntry band_fields[] = {
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
extern const FieldEntry steppable_fields[] = {
    {"value", makesdr_SteppableInt64SettingPb_value_tag,  nullptr},
    {"coarse_delta", makesdr_SteppableInt64SettingPb_coarse_delta_tag,  nullptr},
    {"fine_delta", makesdr_SteppableInt64SettingPb_fine_delta_tag,  nullptr},
    {"use_fine", makesdr_SteppableInt64SettingPb_use_fine_tag,  nullptr},
    {nullptr, 0,  nullptr}
};

extern const FieldEntry iq_corrections_fields[] = {
  {"amplitude", 1001, steppable_fields},
  {"phase", 1002, steppable_fields},
  {nullptr, 0,  nullptr}
};

extern const FieldEntry band_rf_fields[] = {
  {"frequency", makesdr_BandRfSettingsPb_frequency_tag,  steppable_fields},
  {"gain", makesdr_BandRfSettingsPb_gain_tag,  steppable_fields},
  {nullptr, 0,  nullptr}
};

extern const FieldEntry pipeline_rf_fields[] = {
  {"frequency", makesdr_PipelineRfSettingsPb_frequency_tag,  steppable_fields},
  {"max_negative_offset", makesdr_PipelineRfSettingsPb_max_negative_offset_tag,  nullptr},
  {"max_positive_offset", makesdr_PipelineRfSettingsPb_max_positive_offset_tag,  nullptr},
  {nullptr, 0,  nullptr}
};

extern const FieldEntry if_fields[] = {
  {"gain", makesdr_IfSettingsPb_gain_tag,  steppable_fields},
  {"bandwidth", makesdr_IfSettingsPb_bandwidth_tag,  nullptr},
  {nullptr, 0,  nullptr}
};

extern const FieldEntry af_fields[] = {
  {"gain", makesdr_AfSettingsPb_gain_tag,  steppable_fields},
  {nullptr, 0,  nullptr}
};

extern const FieldEntry receiver_fields[] = {
  {"volume", makesdr_ReceiverSettingsPb_volume_tag,  af_fields},
  {nullptr, 0,  nullptr}
};
