#include "DefaultRadioSettings.h"

RadioSettings_RadioSettingsPb defaultRadioSettingsPb = {
  .has_receiver = true,
  .receiver =  {
    .has_iq_corrections = true,
    .iq_corrections =  {
      .has_amplitude = true,
      .amplitude =  {
        .value =  0.0,
        .coarse_delta =  0.01,
        .fine_delta =  0.001
      },
      .has_phase = true,
      .phase =  {
        .value =  0.0,
        .coarse_delta =  0.01,
        .fine_delta =  0.001
      }
    }
  },
  .has_transmitter = true,
  .transmitter =  {
    .has_iq_corrections = true,
    .iq_corrections =  {
      .has_amplitude = true,
      .amplitude =  {
        .value =  0.0,
        .coarse_delta =  0.01,
        .fine_delta =  0.001
      },
      .has_phase = true,
      .phase =  {
        .value =  0.0,
        .coarse_delta =  0.01,
        .fine_delta =  0.001
      }
    },
    .has_mic = true,
    .mic =  {
      .has_gain = true,
      .gain =  {
        .value =  1.0,
        .coarse_delta =  0.01,
        .fine_delta =  0.001
      }
    },
  },
  .has_active_bands = true,
  .active_bands =  {
    .has_focus_band_id = true,
    .focus_band_id =  RadioSettings_SplitBandId_SPLIT_BAND_ONE,
    .has_tx_band_id = true,
    .tx_band_id =  RadioSettings_SplitBandId_SPLIT_BAND_ONE,
    .has_rx_band_id = true,
    .rx_band_id =  RadioSettings_SplitBandId_SPLIT_BAND_ONE,
    .has_band_1 = true,
    .band_1 =   {
      .has_name = true,
      .name = "20m",
      .has_pipeline_a = true,
      .pipeline_a =  {
        .has_mode = true,
        .mode =  RadioSettings_ModeType_MODE_USB,
        .has_rf = true,
        .rf =  {
          .has_max_negative_vfo_offset = false,
          .max_negative_vfo_offset = 0,
          .has_max_positive_vfo_offset = false,
          .max_positive_vfo_offset = 0,
          .has_gain = true,
          .gain =  {
            .value =  1.0,
            .coarse_delta =  0.01,
            .fine_delta =  0.001
          },
          .has_centre_frequency = true,
          .centre_frequency =  {
            .value =  14200000,
            .coarse_delta =  1000,
            .fine_delta =  100
          },
          .has_vfo = true,
          .vfo =  {
            .value =  0,
            .coarse_delta =  1000,
            .fine_delta =  100
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
        .has_mute = true,
        .mute =  false,
        .has_agc_speed = true,
        .agc_speed = RadioSettings_AgcSpeed_AGC_FAST
      },
      .has_pipeline_b = true,
      .pipeline_b =  {
        .has_mode = true,
        .mode =  RadioSettings_ModeType_MODE_USB,
        .has_rf = true,
        .rf =  {
          .has_max_negative_vfo_offset = false,
          .max_negative_vfo_offset = 0,
          .has_max_positive_vfo_offset = false,
          .max_positive_vfo_offset = 0,
          .has_gain = true,
          .gain =  {
            .value =  1.0,
            .coarse_delta =  0.01,
            .fine_delta =  0.001
          },
          .has_centre_frequency = true,
          .centre_frequency =  {
            .value =  14200000,
            .coarse_delta =  1000,
            .fine_delta =  100
          },
          .has_vfo = true,
          .vfo =  {
            .value =  0,
            .coarse_delta =  1000,
            .fine_delta =  100
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
        .has_mute = true,
        .mute =  false,
        .has_agc_speed = true,
        .agc_speed = RadioSettings_AgcSpeed_AGC_FAST
      },
      .has_tx_pipeline = true,
      .tx_pipeline =  {
        .has_mode = true,
        .mode =  RadioSettings_ModeType_MODE_USB,
        .has_rf = true,
        .rf =  {
          .has_max_negative_vfo_offset = false,
          .max_negative_vfo_offset = 0,
          .has_max_positive_vfo_offset = false,
          .max_positive_vfo_offset = 0,
          .has_gain = true,
          .gain =  {
            .value =  1.0,
            .coarse_delta =  0.01,
            .fine_delta =  0.001
          },
          .centre_frequency =  {
            .value =  14200000,
            .coarse_delta =  1000,
            .fine_delta =  100
          },
          .has_vfo = true,
          .vfo =  {
            .value =  0,
            .coarse_delta =  1000,
            .fine_delta =  100
          }
        }
      },
      .has_is_multi_pipeline = true,
      .is_multi_pipeline =  true,
      .has_focus_pipeline_id = true,
      .focus_pipeline_id =  RadioSettings_PipelineId_PIPELINE_A,
      .has_tx_pipeline_id = true,
      .tx_pipeline_id =  RadioSettings_PipelineId_PIPELINE_A
    },
    .has_band_2 = true,
    .band_2 =  {
      .has_name = true,
      .name = "40m",
      .has_pipeline_a = true,
      .pipeline_a =  {
        .has_mode = true,
        .mode =  RadioSettings_ModeType_MODE_LSB,
        .has_rf = true,
        .rf =  {
          .has_max_negative_vfo_offset = false,
          .max_negative_vfo_offset = 0,
          .has_max_positive_vfo_offset = false,
          .max_positive_vfo_offset = 0,
          .has_gain = true,
          .gain =  {
            .value =  1.0,
            .coarse_delta =  0.01,
            .fine_delta =  0.001
          },
          .has_centre_frequency = true,
          .centre_frequency =  {
            .value =  7100000,
            .coarse_delta =  1000,
            .fine_delta =  100
          },
          .has_vfo = true,
          .vfo =  {
            .value =  7130000,
            .coarse_delta =  1000,
            .fine_delta =  100
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
        .has_mute = true,
        .mute =  false,
        .has_agc_speed = true,
        .agc_speed = RadioSettings_AgcSpeed_AGC_FAST
      },
      .has_pipeline_b = true,
      .pipeline_b =  {
        .has_mode = true,
        .mode =  RadioSettings_ModeType_MODE_LSB,
        .has_rf = true,
        .rf =  {
          .has_max_negative_vfo_offset = false,
          .max_negative_vfo_offset = 0,
          .has_max_positive_vfo_offset = false,
          .max_positive_vfo_offset = 0,
          .has_gain = true,
          .gain =  {
            .value =  1.0,
            .coarse_delta =  0.01,
            .fine_delta =  0.001
          },
          .has_centre_frequency = true,
          .centre_frequency =  {
            .value =  7100000,
            .coarse_delta =  1000,
            .fine_delta =  100
          },
          .has_vfo = true,
          .vfo =  {
            .value =  7140000,
            .coarse_delta =  1000,
            .fine_delta =  100
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
        .has_mute = true,
        .mute =  false,
        .has_agc_speed = true,
        .agc_speed = RadioSettings_AgcSpeed_AGC_FAST
      },
      .has_tx_pipeline = true,
      .tx_pipeline =  {
        .has_mode = true,
        .mode =  RadioSettings_ModeType_MODE_LSB,
        .has_rf = true,
        .rf =  {
          .has_max_negative_vfo_offset = false,
          .max_negative_vfo_offset = 0,
          .has_max_positive_vfo_offset = false,
          .max_positive_vfo_offset = 0,
          .has_gain = true,
          .gain =  {
            .value =  1.0,
            .coarse_delta =  0.01,
            .fine_delta =  0.001
          },
          .has_centre_frequency = true,
          .centre_frequency =  {
            .value =  7100000,
            .coarse_delta =  1000,
            .fine_delta =  100
          },
          .has_vfo = true,
          .vfo =  {
            .value =  7130000,
            .coarse_delta =  1000,
            .fine_delta =  100
          }
        }
      },
      .has_is_multi_pipeline = true,
      .is_multi_pipeline =  true,
      .has_focus_pipeline_id = true,
      .focus_pipeline_id =  RadioSettings_PipelineId_PIPELINE_A,
      .has_tx_pipeline_id = true,
      .tx_pipeline_id =  RadioSettings_PipelineId_PIPELINE_A
    }
  },
  .has_ptt = true,
  .ptt =  false
};
