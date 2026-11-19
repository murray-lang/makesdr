#include "test/settings/testRadioSettings.h"

// constexpr so that every object initialised from it is constant-initialised
// (baked into .data). Copying a plain non-const global would make
// testRadioSettingsPayloadPb dynamically initialised, and globals in other
// TUs (e.g. `settings` in the CM4 main.cpp) could then read it before .body
// has been filled in - the static initialisation order fiasco.
static constexpr makesdr_SingleIqRxSettingsPb constTestRadioSettingsPb = {
  .has_active_bands = true,
  .active_bands =  {
    .has_focus_band = true,
    .focus_band =   {
      .which_band_or_request = makesdr_SingleIqBandSettingsPb_band_request_tag,
      .band_or_request {
        .band_request = "20m",
      },
      .has_rf = true,
      .rf =  {
        .has_frequency = true,
        .frequency =  {
          .value =  14200000,
          .coarse_delta =  50000,
          .fine_delta =  10000,
        },
        .has_gain = true,
        .gain =  {
          .value =  0.0,
          .coarse_delta =  0.01,
          .fine_delta =  0.001
        }
      },
      .has_if_ = true,
        .if_ =  {
          .has_bandwidth = true,
          .bandwidth = 200000,
          .has_gain = true,
          .gain =  {
            .value =  0.0,
            .coarse_delta =  0.01,
            .fine_delta =  0.001
          },

        },
      .has_focus_pipeline = true,
      .focus_pipeline =  {
        .has_base = true,
        .base = {
          .which_mode_or_request = makesdr_PipelineSettingsPb_mode_request_tag,
          .mode_or_request =  {
            .mode_request =  makesdr_ModeType_MODE_AMN,
          },
          .has_rf = true,
          .rf =  {
            .has_max_negative_offset = false,
            .max_negative_offset = 0,
            .has_max_positive_offset = false,
            .max_positive_offset = 0,
            .has_frequency = true,
            .frequency =  {
              .value =  14220000,
              .coarse_delta =  1000,
              .fine_delta =  100,
              .use_fine = false
            }
          }
        },
        .has_mute = true,
        .mute =  false,
        .has_agc_speed = true,
        .agc_speed = makesdr_AgcSpeed_AGC_FAST
      },
    }
  },
  .has_receiver = true,
  .receiver =  {
    .has_volume = true,
    .volume =  {
      .has_gain = true,
      .gain =  {
        .value =  1.0,
        .coarse_delta =  0.01,
        .fine_delta =  0.001
      }
    },
  },
  .has_ptt = true,
  .ptt =  false
};

makesdr_SingleIqRxSettingsPb testRadioSettingsPb = constTestRadioSettingsPb;

makesdr_SingleIqRxSettingsPayloadPb testRadioSettingsPayloadPb = {
  .has_header = true,
  .header = {
    .payloadType = makesdr_RadioPayloadType_PAYLOAD_SETTINGS_SINGLE_IQ_RX,
    .source = makesdr_RadioPayloadSource_SOURCE_BACK_END,
    .purpose = makesdr_RadioPayloadPurpose_PURPOSE_REPLACE
  },

  .has_body = true,
  .body =  constTestRadioSettingsPb
};
