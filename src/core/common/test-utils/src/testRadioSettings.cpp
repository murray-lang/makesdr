
#include "test-utils/testRadioSettings.h"

makesdr_SplitBandDualIqRxTxSettingsPayloadPb testRadioSettingsPayloadPb = {
  .has_header = true,
  .header = {
    .payloadType = makesdr_RadioPayloadType_PAYLOAD_SETTINGS_SPLIT_BAND_DUAL_IQ_RXTX,
    .source = makesdr_RadioPayloadSource_SOURCE_BACK_END,
    .purpose = makesdr_RadioPayloadPurpose_PURPOSE_REPLACE
  },

  .has_body = true,
  .body =  testRadioSettingsPb
};

makesdr_SplitBandDualIqRxTxSettingsPb testRadioSettingsPb = {
  .has_active_bands = true,
  .active_bands =  {
    .focus_band = makesdr_SplitBandId_SPLIT_BAND_ONE, // Pseudo band, but initialise anyway
    .has_band_1 = true,
    .band_1 =   {
      .which_band_or_request = makesdr_DualIqBandSettingsPb_band_request_tag,
      .band_or_request {
        .band_request = "20m",
      },
      .has_rf = true,
      .rf =  {
        .has_frequency = true,
        .frequency =  {
          .value =  14200000,
          .coarse_delta =  1000,
          .fine_delta =  100
        },
        .has_gain = true,
        .gain =  {
          .value =  1.0,
          .coarse_delta =  0.01,
          .fine_delta =  0.001
        }
      },
      .has_if_ = true,
        .if_ =  {
          .has_gain = true,
          .gain =  {
            .value =  1.0,
            .coarse_delta =  0.01,
            .fine_delta =  0.001
          }
        },
      .has_pipeline_a = true,
      .pipeline_a =  {
        .has_base = true,
        .base = {
          .which_mode_or_request = makesdr_PipelineSettingsPb_mode_request_tag,
          .mode_or_request =  {
            .mode_request =  makesdr_ModeType_MODE_USB,
          },
          .has_rf = true,
          .rf =  {
            .has_max_negative_offset = false,
            .max_negative_offset = 0,
            .has_max_positive_offset = false,
            .max_positive_offset = 0,
            .has_frequency = true,
            .frequency =  {
              .value =  14200000,
              .coarse_delta =  1000,
              .fine_delta =  100
            }
          }
        },
        .has_mute = true,
        .mute =  false,
        .has_agc_speed = true,
        .agc_speed = makesdr_AgcSpeed_AGC_FAST
      },
      .has_pipeline_b = true,
      .pipeline_b =  {
        .has_base = true,
        .base = {
          .which_mode_or_request = makesdr_PipelineSettingsPb_mode_request_tag,
          .mode_or_request =  {
            .mode_request =  makesdr_ModeType_MODE_USB,
          },
          .has_rf = true,
          .rf =  {
            .has_max_negative_offset = false,
            .max_negative_offset = 0,
            .has_max_positive_offset = false,
            .max_positive_offset = 0,
            .has_frequency = true,
            .frequency =  {
              .value =  14200000,
              .coarse_delta =  1000,
              .fine_delta =  100
            },
          }
        },
        .has_mute = true,
        .mute =  false,
        .has_agc_speed = true,
        .agc_speed = makesdr_AgcSpeed_AGC_FAST
      },
      .has_is_multi_pipeline = true,
      .is_multi_pipeline =  true,
      .has_focus_pipeline_id = true,
      .focus_pipeline_id =  makesdr_PipelineId_PIPELINE_A,
      .has_tx_pipeline = true,
      .tx_pipeline =  {
        .has_base = true,
        .base = {
          .which_mode_or_request = makesdr_PipelineSettingsPb_mode_request_tag,
          .mode_or_request =  {
            .mode_request =  makesdr_ModeType_MODE_USB,
          },
          .has_rf = true,
          .rf =  {
            .has_max_negative_offset = false,
            .max_negative_offset = 0,
            .has_max_positive_offset = false,
            .max_positive_offset = 0,
            .has_frequency = true,
            .frequency =  {
              .value =  14200000,
              .coarse_delta =  1000,
              .fine_delta =  100
            }
          }
        }

      },
      .has_tx_pipeline_id = true,
      .tx_pipeline_id =  makesdr_PipelineId_PIPELINE_A
    },
    .has_band_2 = true,
    .band_2 =  {
      .which_band_or_request = makesdr_DualIqBandSettingsPb_band_request_tag,
      .band_or_request {
        .band_request = "40m",
      },
      .has_pipeline_a = true,
      .pipeline_a =  {
        .has_base = true,
        .base = {
          .which_mode_or_request = makesdr_PipelineSettingsPb_mode_request_tag,
          .mode_or_request =  {
            .mode_request =  makesdr_ModeType_MODE_LSB,
          },
          .has_rf = true,
          .rf =  {
            .has_max_negative_offset = false,
            .max_negative_offset = 0,
            .has_max_positive_offset = false,
            .max_positive_offset = 0,
            .has_frequency = true,
            .frequency =  {
              .value =  7100000,
              .coarse_delta =  1000,
              .fine_delta =  100
            }
          }
        },
        .has_mute = true,
        .mute =  false,
        .has_agc_speed = true,
        .agc_speed = makesdr_AgcSpeed_AGC_FAST
      },
      .has_pipeline_b = true,
      .pipeline_b =  {
        .has_base = true,
        .base = {
          .which_mode_or_request = makesdr_PipelineSettingsPb_mode_request_tag,
          .mode_or_request =  {
            .mode_request =  makesdr_ModeType_MODE_LSB,
          },
          .has_rf = true,
          .rf =  {
            .has_max_negative_offset = false,
            .max_negative_offset = 0,
            .has_max_positive_offset = false,
            .max_positive_offset = 0,
            .has_frequency = true,
            .frequency =  {
              .value =  7100000,
              .coarse_delta =  1000,
              .fine_delta =  100
            }
          }
        },
        .has_mute = true,
        .mute =  false,
        .has_agc_speed = true,
        .agc_speed = makesdr_AgcSpeed_AGC_FAST
      },
      .has_is_multi_pipeline = true,
      .is_multi_pipeline =  true,
      .has_focus_pipeline_id = true,
      .focus_pipeline_id =  makesdr_PipelineId_PIPELINE_A,
      .has_tx_pipeline = true,
      .tx_pipeline =  {
        .has_base = true,
        .base = {
          .which_mode_or_request = makesdr_PipelineSettingsPb_mode_request_tag,
          .mode_or_request =  {
            .mode_request =  makesdr_ModeType_MODE_LSB,
          },
          .has_rf = true,
          .rf =  {
            .has_max_negative_offset = false,
            .max_negative_offset = 0,
            .has_max_positive_offset = false,
            .max_positive_offset = 0,
            .has_frequency = true,
            .frequency =  {
              .value =  7100000,
              .coarse_delta =  1000,
              .fine_delta =  100
            }
          }
        }
      },
      .has_tx_pipeline_id = true,
      .tx_pipeline_id =  makesdr_PipelineId_PIPELINE_A
    },
    .has_focus_band_id = true,
    .focus_band_id =  makesdr_SplitBandId_SPLIT_BAND_ONE,
    .has_tx_band_id = true,
    .tx_band_id =  makesdr_SplitBandId_SPLIT_BAND_ONE,
    .has_rx_band_id = true,
    .rx_band_id =  makesdr_SplitBandId_SPLIT_BAND_ONE,
    .is_split = false
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
  .has_transmitter = true,
  .transmitter =  {
    .has_mic = true,
    .mic =  {
      .has_gain = true,
      .gain =  {
        .value =  1.0,
        .coarse_delta =  0.01,
        .fine_delta =  0.001
      }
    }
  },
  .has_ptt = true,
  .ptt =  false
};
