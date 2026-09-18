#include "SplitBandDualIqResolved.h"

// Pre-resolved field descriptors (no string processing at runtime)
// Uncomment the ones you need, likewise in SplitBandDualIqResolved.h
/*
const FieldDescriptor active_bands = FieldDescriptor(
    FieldPath{1},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_focus_band = FieldDescriptor(
    FieldPath{1, 1},
    false,
    true
);
*/
const FieldDescriptor active_bands_focus_band_band_request = FieldDescriptor(
    FieldPath{1, 1, 1},
    true,
    true
);
/*
const FieldDescriptor active_bands_focus_band_band = FieldDescriptor(
    FieldPath{1, 1, 2},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_band_name = FieldDescriptor(
    FieldPath{1, 1, 2, 1},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_band_label = FieldDescriptor(
    FieldPath{1, 1, 2, 2},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_band_lowest_frequency = FieldDescriptor(
    FieldPath{1, 1, 2, 3},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_band_highest_frequency = FieldDescriptor(
    FieldPath{1, 1, 2, 4},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_band_landing_frequency = FieldDescriptor(
    FieldPath{1, 1, 2, 5},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_band_default_fine_step = FieldDescriptor(
    FieldPath{1, 1, 2, 6},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_band_default_coarse_step = FieldDescriptor(
    FieldPath{1, 1, 2, 7},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_band_default_mode = FieldDescriptor(
    FieldPath{1, 1, 2, 8},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_band_default_mode_type = FieldDescriptor(
    FieldPath{1, 1, 2, 8, 1},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_band_default_mode_name = FieldDescriptor(
    FieldPath{1, 1, 2, 8, 2},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_band_default_mode_label = FieldDescriptor(
    FieldPath{1, 1, 2, 8, 3},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_band_default_mode_lo_cut = FieldDescriptor(
    FieldPath{1, 1, 2, 8, 4},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_band_default_mode_hi_cut = FieldDescriptor(
    FieldPath{1, 1, 2, 8, 5},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_band_default_mode_offset = FieldDescriptor(
    FieldPath{1, 1, 2, 8, 6},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_rf = FieldDescriptor(
    FieldPath{1, 1, 5},
    false,
    true
);
*/
const FieldDescriptor active_bands_focus_band_rf_frequency = FieldDescriptor(
    FieldPath{1, 1, 5, 1001},
    false,
    true
);
/*
const FieldDescriptor active_bands_focus_band_rf_frequency_value = FieldDescriptor(
    FieldPath{1, 1, 5, 1001, 1},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_rf_frequency_coarse_delta = FieldDescriptor(
    FieldPath{1, 1, 5, 1001, 2},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_rf_frequency_fine_delta = FieldDescriptor(
    FieldPath{1, 1, 5, 1001, 3},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_rf_frequency_use_fine = FieldDescriptor(
    FieldPath{1, 1, 5, 1001, 4},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_rf_gain = FieldDescriptor(
    FieldPath{1, 1, 5, 1002},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_rf_gain_value = FieldDescriptor(
    FieldPath{1, 1, 5, 1002, 1},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_rf_gain_coarse_delta = FieldDescriptor(
    FieldPath{1, 1, 5, 1002, 2},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_rf_gain_fine_delta = FieldDescriptor(
    FieldPath{1, 1, 5, 1002, 3},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_rf_gain_use_fine = FieldDescriptor(
    FieldPath{1, 1, 5, 1002, 4},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_if = FieldDescriptor(
    FieldPath{1, 1, 6},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_if_gain = FieldDescriptor(
    FieldPath{1, 1, 6, 1001},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_if_gain_value = FieldDescriptor(
    FieldPath{1, 1, 6, 1001, 1},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_if_gain_coarse_delta = FieldDescriptor(
    FieldPath{1, 1, 6, 1001, 2},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_if_gain_fine_delta = FieldDescriptor(
    FieldPath{1, 1, 6, 1001, 3},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_if_gain_use_fine = FieldDescriptor(
    FieldPath{1, 1, 6, 1001, 4},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_if_bandwidth = FieldDescriptor(
    FieldPath{1, 1, 6, 2},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_focus_pipeline = FieldDescriptor(
    FieldPath{1, 1, 10},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_focus_pipeline_base = FieldDescriptor(
    FieldPath{1, 1, 10, 1},
    false,
    true
);
*/
const FieldDescriptor active_bands_focus_band_focus_pipeline_base_mode_request = FieldDescriptor(
    FieldPath{1, 1, 10, 1, 1},
    true,
    true
);
/*
const FieldDescriptor active_bands_focus_band_focus_pipeline_base_mode = FieldDescriptor(
    FieldPath{1, 1, 10, 1, 2},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_focus_pipeline_base_mode_type = FieldDescriptor(
    FieldPath{1, 1, 10, 1, 2, 1},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_focus_pipeline_base_mode_name = FieldDescriptor(
    FieldPath{1, 1, 10, 1, 2, 2},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_focus_pipeline_base_mode_label = FieldDescriptor(
    FieldPath{1, 1, 10, 1, 2, 3},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_focus_pipeline_base_mode_lo_cut = FieldDescriptor(
    FieldPath{1, 1, 10, 1, 2, 4},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_focus_pipeline_base_mode_hi_cut = FieldDescriptor(
    FieldPath{1, 1, 10, 1, 2, 5},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_focus_pipeline_base_mode_offset = FieldDescriptor(
    FieldPath{1, 1, 10, 1, 2, 6},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_focus_pipeline_base_rf = FieldDescriptor(
    FieldPath{1, 1, 10, 1, 3},
    false,
    true
);
*/
const FieldDescriptor active_bands_focus_band_focus_pipeline_base_rf_frequency = FieldDescriptor(
    FieldPath{1, 1, 10, 1, 3, 1001},
    false,
    true
);
/*
const FieldDescriptor active_bands_focus_band_focus_pipeline_base_rf_frequency_value = FieldDescriptor(
    FieldPath{1, 1, 10, 1, 3, 1001, 1},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_focus_pipeline_base_rf_frequency_coarse_delta = FieldDescriptor(
    FieldPath{1, 1, 10, 1, 3, 1001, 2},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_focus_pipeline_base_rf_frequency_fine_delta = FieldDescriptor(
    FieldPath{1, 1, 10, 1, 3, 1001, 3},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_focus_pipeline_base_rf_frequency_use_fine = FieldDescriptor(
    FieldPath{1, 1, 10, 1, 3, 1001, 4},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_focus_pipeline_base_rf_max_negative_offset = FieldDescriptor(
    FieldPath{1, 1, 10, 1, 3, 4},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_focus_pipeline_base_rf_max_positive_offset = FieldDescriptor(
    FieldPath{1, 1, 10, 1, 3, 5},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_focus_pipeline_base_iq_corrections = FieldDescriptor(
    FieldPath{1, 1, 10, 1, 4},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_focus_pipeline_base_iq_corrections_amplitude = FieldDescriptor(
    FieldPath{1, 1, 10, 1, 4, 1001},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_focus_pipeline_base_iq_corrections_amplitude_value = FieldDescriptor(
    FieldPath{1, 1, 10, 1, 4, 1001, 1},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_focus_pipeline_base_iq_corrections_amplitude_coarse_delta = FieldDescriptor(
    FieldPath{1, 1, 10, 1, 4, 1001, 2},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_focus_pipeline_base_iq_corrections_amplitude_fine_delta = FieldDescriptor(
    FieldPath{1, 1, 10, 1, 4, 1001, 3},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_focus_pipeline_base_iq_corrections_amplitude_use_fine = FieldDescriptor(
    FieldPath{1, 1, 10, 1, 4, 1001, 4},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_focus_pipeline_base_iq_corrections_phase = FieldDescriptor(
    FieldPath{1, 1, 10, 1, 4, 1002},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_focus_pipeline_base_iq_corrections_phase_value = FieldDescriptor(
    FieldPath{1, 1, 10, 1, 4, 1002, 1},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_focus_pipeline_base_iq_corrections_phase_coarse_delta = FieldDescriptor(
    FieldPath{1, 1, 10, 1, 4, 1002, 2},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_focus_pipeline_base_iq_corrections_phase_fine_delta = FieldDescriptor(
    FieldPath{1, 1, 10, 1, 4, 1002, 3},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_focus_pipeline_base_iq_corrections_phase_use_fine = FieldDescriptor(
    FieldPath{1, 1, 10, 1, 4, 1002, 4},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_focus_pipeline_mute = FieldDescriptor(
    FieldPath{1, 1, 10, 2},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_focus_pipeline_agc_speed = FieldDescriptor(
    FieldPath{1, 1, 10, 3},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_pipeline_a = FieldDescriptor(
    FieldPath{1, 1, 11},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_pipeline_a_base = FieldDescriptor(
    FieldPath{1, 1, 11, 1},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_pipeline_a_base_mode_request = FieldDescriptor(
    FieldPath{1, 1, 11, 1, 1},
    true,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_pipeline_a_base_mode = FieldDescriptor(
    FieldPath{1, 1, 11, 1, 2},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_pipeline_a_base_mode_type = FieldDescriptor(
    FieldPath{1, 1, 11, 1, 2, 1},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_pipeline_a_base_mode_name = FieldDescriptor(
    FieldPath{1, 1, 11, 1, 2, 2},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_pipeline_a_base_mode_label = FieldDescriptor(
    FieldPath{1, 1, 11, 1, 2, 3},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_pipeline_a_base_mode_lo_cut = FieldDescriptor(
    FieldPath{1, 1, 11, 1, 2, 4},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_pipeline_a_base_mode_hi_cut = FieldDescriptor(
    FieldPath{1, 1, 11, 1, 2, 5},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_pipeline_a_base_mode_offset = FieldDescriptor(
    FieldPath{1, 1, 11, 1, 2, 6},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_pipeline_a_base_rf = FieldDescriptor(
    FieldPath{1, 1, 11, 1, 3},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_pipeline_a_base_rf_frequency = FieldDescriptor(
    FieldPath{1, 1, 11, 1, 3, 1001},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_pipeline_a_base_rf_frequency_value = FieldDescriptor(
    FieldPath{1, 1, 11, 1, 3, 1001, 1},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_pipeline_a_base_rf_frequency_coarse_delta = FieldDescriptor(
    FieldPath{1, 1, 11, 1, 3, 1001, 2},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_pipeline_a_base_rf_frequency_fine_delta = FieldDescriptor(
    FieldPath{1, 1, 11, 1, 3, 1001, 3},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_pipeline_a_base_rf_frequency_use_fine = FieldDescriptor(
    FieldPath{1, 1, 11, 1, 3, 1001, 4},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_pipeline_a_base_rf_max_negative_offset = FieldDescriptor(
    FieldPath{1, 1, 11, 1, 3, 4},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_pipeline_a_base_rf_max_positive_offset = FieldDescriptor(
    FieldPath{1, 1, 11, 1, 3, 5},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_pipeline_a_base_iq_corrections = FieldDescriptor(
    FieldPath{1, 1, 11, 1, 4},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_pipeline_a_base_iq_corrections_amplitude = FieldDescriptor(
    FieldPath{1, 1, 11, 1, 4, 1001},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_pipeline_a_base_iq_corrections_amplitude_value = FieldDescriptor(
    FieldPath{1, 1, 11, 1, 4, 1001, 1},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_pipeline_a_base_iq_corrections_amplitude_coarse_delta = FieldDescriptor(
    FieldPath{1, 1, 11, 1, 4, 1001, 2},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_pipeline_a_base_iq_corrections_amplitude_fine_delta = FieldDescriptor(
    FieldPath{1, 1, 11, 1, 4, 1001, 3},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_pipeline_a_base_iq_corrections_amplitude_use_fine = FieldDescriptor(
    FieldPath{1, 1, 11, 1, 4, 1001, 4},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_pipeline_a_base_iq_corrections_phase = FieldDescriptor(
    FieldPath{1, 1, 11, 1, 4, 1002},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_pipeline_a_base_iq_corrections_phase_value = FieldDescriptor(
    FieldPath{1, 1, 11, 1, 4, 1002, 1},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_pipeline_a_base_iq_corrections_phase_coarse_delta = FieldDescriptor(
    FieldPath{1, 1, 11, 1, 4, 1002, 2},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_pipeline_a_base_iq_corrections_phase_fine_delta = FieldDescriptor(
    FieldPath{1, 1, 11, 1, 4, 1002, 3},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_pipeline_a_base_iq_corrections_phase_use_fine = FieldDescriptor(
    FieldPath{1, 1, 11, 1, 4, 1002, 4},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_pipeline_a_mute = FieldDescriptor(
    FieldPath{1, 1, 11, 2},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_pipeline_a_agc_speed = FieldDescriptor(
    FieldPath{1, 1, 11, 3},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_pipeline_b = FieldDescriptor(
    FieldPath{1, 1, 12},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_pipeline_b_base = FieldDescriptor(
    FieldPath{1, 1, 12, 1},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_pipeline_b_base_mode_request = FieldDescriptor(
    FieldPath{1, 1, 12, 1, 1},
    true,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_pipeline_b_base_mode = FieldDescriptor(
    FieldPath{1, 1, 12, 1, 2},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_pipeline_b_base_mode_type = FieldDescriptor(
    FieldPath{1, 1, 12, 1, 2, 1},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_pipeline_b_base_mode_name = FieldDescriptor(
    FieldPath{1, 1, 12, 1, 2, 2},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_pipeline_b_base_mode_label = FieldDescriptor(
    FieldPath{1, 1, 12, 1, 2, 3},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_pipeline_b_base_mode_lo_cut = FieldDescriptor(
    FieldPath{1, 1, 12, 1, 2, 4},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_pipeline_b_base_mode_hi_cut = FieldDescriptor(
    FieldPath{1, 1, 12, 1, 2, 5},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_pipeline_b_base_mode_offset = FieldDescriptor(
    FieldPath{1, 1, 12, 1, 2, 6},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_pipeline_b_base_rf = FieldDescriptor(
    FieldPath{1, 1, 12, 1, 3},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_pipeline_b_base_rf_frequency = FieldDescriptor(
    FieldPath{1, 1, 12, 1, 3, 1001},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_pipeline_b_base_rf_frequency_value = FieldDescriptor(
    FieldPath{1, 1, 12, 1, 3, 1001, 1},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_pipeline_b_base_rf_frequency_coarse_delta = FieldDescriptor(
    FieldPath{1, 1, 12, 1, 3, 1001, 2},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_pipeline_b_base_rf_frequency_fine_delta = FieldDescriptor(
    FieldPath{1, 1, 12, 1, 3, 1001, 3},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_pipeline_b_base_rf_frequency_use_fine = FieldDescriptor(
    FieldPath{1, 1, 12, 1, 3, 1001, 4},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_pipeline_b_base_rf_max_negative_offset = FieldDescriptor(
    FieldPath{1, 1, 12, 1, 3, 4},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_pipeline_b_base_rf_max_positive_offset = FieldDescriptor(
    FieldPath{1, 1, 12, 1, 3, 5},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_pipeline_b_base_iq_corrections = FieldDescriptor(
    FieldPath{1, 1, 12, 1, 4},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_pipeline_b_base_iq_corrections_amplitude = FieldDescriptor(
    FieldPath{1, 1, 12, 1, 4, 1001},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_pipeline_b_base_iq_corrections_amplitude_value = FieldDescriptor(
    FieldPath{1, 1, 12, 1, 4, 1001, 1},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_pipeline_b_base_iq_corrections_amplitude_coarse_delta = FieldDescriptor(
    FieldPath{1, 1, 12, 1, 4, 1001, 2},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_pipeline_b_base_iq_corrections_amplitude_fine_delta = FieldDescriptor(
    FieldPath{1, 1, 12, 1, 4, 1001, 3},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_pipeline_b_base_iq_corrections_amplitude_use_fine = FieldDescriptor(
    FieldPath{1, 1, 12, 1, 4, 1001, 4},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_pipeline_b_base_iq_corrections_phase = FieldDescriptor(
    FieldPath{1, 1, 12, 1, 4, 1002},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_pipeline_b_base_iq_corrections_phase_value = FieldDescriptor(
    FieldPath{1, 1, 12, 1, 4, 1002, 1},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_pipeline_b_base_iq_corrections_phase_coarse_delta = FieldDescriptor(
    FieldPath{1, 1, 12, 1, 4, 1002, 2},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_pipeline_b_base_iq_corrections_phase_fine_delta = FieldDescriptor(
    FieldPath{1, 1, 12, 1, 4, 1002, 3},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_pipeline_b_base_iq_corrections_phase_use_fine = FieldDescriptor(
    FieldPath{1, 1, 12, 1, 4, 1002, 4},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_pipeline_b_mute = FieldDescriptor(
    FieldPath{1, 1, 12, 2},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_pipeline_b_agc_speed = FieldDescriptor(
    FieldPath{1, 1, 12, 3},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_is_multi_pipeline = FieldDescriptor(
    FieldPath{1, 1, 13},
    true,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_focus_pipeline_id = FieldDescriptor(
    FieldPath{1, 1, 14},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_tx_pipeline = FieldDescriptor(
    FieldPath{1, 1, 15},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_tx_pipeline_base = FieldDescriptor(
    FieldPath{1, 1, 15, 1},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_tx_pipeline_base_mode_request = FieldDescriptor(
    FieldPath{1, 1, 15, 1, 1},
    true,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_tx_pipeline_base_mode = FieldDescriptor(
    FieldPath{1, 1, 15, 1, 2},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_tx_pipeline_base_mode_type = FieldDescriptor(
    FieldPath{1, 1, 15, 1, 2, 1},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_tx_pipeline_base_mode_name = FieldDescriptor(
    FieldPath{1, 1, 15, 1, 2, 2},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_tx_pipeline_base_mode_label = FieldDescriptor(
    FieldPath{1, 1, 15, 1, 2, 3},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_tx_pipeline_base_mode_lo_cut = FieldDescriptor(
    FieldPath{1, 1, 15, 1, 2, 4},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_tx_pipeline_base_mode_hi_cut = FieldDescriptor(
    FieldPath{1, 1, 15, 1, 2, 5},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_tx_pipeline_base_mode_offset = FieldDescriptor(
    FieldPath{1, 1, 15, 1, 2, 6},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_tx_pipeline_base_rf = FieldDescriptor(
    FieldPath{1, 1, 15, 1, 3},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_tx_pipeline_base_rf_frequency = FieldDescriptor(
    FieldPath{1, 1, 15, 1, 3, 1001},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_tx_pipeline_base_rf_frequency_value = FieldDescriptor(
    FieldPath{1, 1, 15, 1, 3, 1001, 1},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_tx_pipeline_base_rf_frequency_coarse_delta = FieldDescriptor(
    FieldPath{1, 1, 15, 1, 3, 1001, 2},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_tx_pipeline_base_rf_frequency_fine_delta = FieldDescriptor(
    FieldPath{1, 1, 15, 1, 3, 1001, 3},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_tx_pipeline_base_rf_frequency_use_fine = FieldDescriptor(
    FieldPath{1, 1, 15, 1, 3, 1001, 4},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_tx_pipeline_base_rf_max_negative_offset = FieldDescriptor(
    FieldPath{1, 1, 15, 1, 3, 4},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_tx_pipeline_base_rf_max_positive_offset = FieldDescriptor(
    FieldPath{1, 1, 15, 1, 3, 5},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_tx_pipeline_base_iq_corrections = FieldDescriptor(
    FieldPath{1, 1, 15, 1, 4},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_tx_pipeline_base_iq_corrections_amplitude = FieldDescriptor(
    FieldPath{1, 1, 15, 1, 4, 1001},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_tx_pipeline_base_iq_corrections_amplitude_value = FieldDescriptor(
    FieldPath{1, 1, 15, 1, 4, 1001, 1},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_tx_pipeline_base_iq_corrections_amplitude_coarse_delta = FieldDescriptor(
    FieldPath{1, 1, 15, 1, 4, 1001, 2},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_tx_pipeline_base_iq_corrections_amplitude_fine_delta = FieldDescriptor(
    FieldPath{1, 1, 15, 1, 4, 1001, 3},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_tx_pipeline_base_iq_corrections_amplitude_use_fine = FieldDescriptor(
    FieldPath{1, 1, 15, 1, 4, 1001, 4},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_tx_pipeline_base_iq_corrections_phase = FieldDescriptor(
    FieldPath{1, 1, 15, 1, 4, 1002},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_tx_pipeline_base_iq_corrections_phase_value = FieldDescriptor(
    FieldPath{1, 1, 15, 1, 4, 1002, 1},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_tx_pipeline_base_iq_corrections_phase_coarse_delta = FieldDescriptor(
    FieldPath{1, 1, 15, 1, 4, 1002, 2},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_tx_pipeline_base_iq_corrections_phase_fine_delta = FieldDescriptor(
    FieldPath{1, 1, 15, 1, 4, 1002, 3},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_tx_pipeline_base_iq_corrections_phase_use_fine = FieldDescriptor(
    FieldPath{1, 1, 15, 1, 4, 1002, 4},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_focus_band_tx_pipeline_id = FieldDescriptor(
    FieldPath{1, 1, 16},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_band_1 = FieldDescriptor(
    FieldPath{1, 1},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_band_request = FieldDescriptor(
    FieldPath{1, 2, 1},
    true,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_band = FieldDescriptor(
    FieldPath{1, 2, 2},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_band_name = FieldDescriptor(
    FieldPath{1, 2, 2, 1},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_band_label = FieldDescriptor(
    FieldPath{1, 2, 2, 2},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_band_lowest_frequency = FieldDescriptor(
    FieldPath{1, 2, 2, 3},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_band_highest_frequency = FieldDescriptor(
    FieldPath{1, 2, 2, 4},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_band_landing_frequency = FieldDescriptor(
    FieldPath{1, 2, 2, 5},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_band_default_fine_step = FieldDescriptor(
    FieldPath{1, 2, 2, 6},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_band_default_coarse_step = FieldDescriptor(
    FieldPath{1, 2, 2, 7},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_band_default_mode = FieldDescriptor(
    FieldPath{1, 2, 2, 8},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_band_default_mode_type = FieldDescriptor(
    FieldPath{1, 2, 2, 8, 1},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_band_default_mode_name = FieldDescriptor(
    FieldPath{1, 2, 2, 8, 2},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_band_default_mode_label = FieldDescriptor(
    FieldPath{1, 2, 2, 8, 3},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_band_default_mode_lo_cut = FieldDescriptor(
    FieldPath{1, 2, 2, 8, 4},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_band_default_mode_hi_cut = FieldDescriptor(
    FieldPath{1, 2, 2, 8, 5},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_band_default_mode_offset = FieldDescriptor(
    FieldPath{1, 2, 2, 8, 6},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_rf = FieldDescriptor(
    FieldPath{1, 1, 5},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_rf_frequency = FieldDescriptor(
    FieldPath{1, 1, 5, 1001},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_rf_frequency_value = FieldDescriptor(
    FieldPath{1, 1, 5, 1001, 1},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_rf_frequency_coarse_delta = FieldDescriptor(
    FieldPath{1, 1, 5, 1001, 2},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_rf_frequency_fine_delta = FieldDescriptor(
    FieldPath{1, 1, 5, 1001, 3},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_rf_frequency_use_fine = FieldDescriptor(
    FieldPath{1, 1, 5, 1001, 4},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_rf_gain = FieldDescriptor(
    FieldPath{1, 1, 5, 1002},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_rf_gain_value = FieldDescriptor(
    FieldPath{1, 1, 5, 1002, 1},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_rf_gain_coarse_delta = FieldDescriptor(
    FieldPath{1, 1, 5, 1002, 2},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_rf_gain_fine_delta = FieldDescriptor(
    FieldPath{1, 1, 5, 1002, 3},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_rf_gain_use_fine = FieldDescriptor(
    FieldPath{1, 1, 5, 1002, 4},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_if = FieldDescriptor(
    FieldPath{1, 1, 6},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_if_gain = FieldDescriptor(
    FieldPath{1, 1, 6, 1001},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_if_gain_value = FieldDescriptor(
    FieldPath{1, 1, 6, 1001, 1},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_if_gain_coarse_delta = FieldDescriptor(
    FieldPath{1, 1, 6, 1001, 2},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_if_gain_fine_delta = FieldDescriptor(
    FieldPath{1, 1, 6, 1001, 3},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_if_gain_use_fine = FieldDescriptor(
    FieldPath{1, 1, 6, 1001, 4},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_if_bandwidth = FieldDescriptor(
    FieldPath{1, 1, 6, 2},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_focus_pipeline = FieldDescriptor(
    FieldPath{1, 1, 10},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_band_1_focus_pipeline_base = FieldDescriptor(
    FieldPath{1, 1, 10, 1},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_band_1_focus_pipeline_base_mode_request = FieldDescriptor(
    FieldPath{1, 1, 10, 1, 1},
    true,
    true
);
*/
/*
const FieldDescriptor active_bands_band_1_focus_pipeline_base_mode = FieldDescriptor(
    FieldPath{1, 1, 10, 1, 2},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_band_1_focus_pipeline_base_mode_type = FieldDescriptor(
    FieldPath{1, 1, 10, 1, 2, 1},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_band_1_focus_pipeline_base_mode_name = FieldDescriptor(
    FieldPath{1, 1, 10, 1, 2, 2},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_band_1_focus_pipeline_base_mode_label = FieldDescriptor(
    FieldPath{1, 1, 10, 1, 2, 3},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_band_1_focus_pipeline_base_mode_lo_cut = FieldDescriptor(
    FieldPath{1, 1, 10, 1, 2, 4},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_band_1_focus_pipeline_base_mode_hi_cut = FieldDescriptor(
    FieldPath{1, 1, 10, 1, 2, 5},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_band_1_focus_pipeline_base_mode_offset = FieldDescriptor(
    FieldPath{1, 1, 10, 1, 2, 6},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_band_1_focus_pipeline_base_rf = FieldDescriptor(
    FieldPath{1, 1, 10, 1, 3},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_band_1_focus_pipeline_base_rf_frequency = FieldDescriptor(
    FieldPath{1, 1, 10, 1, 3, 1001},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_band_1_focus_pipeline_base_rf_frequency_value = FieldDescriptor(
    FieldPath{1, 1, 10, 1, 3, 1001, 1},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_band_1_focus_pipeline_base_rf_frequency_coarse_delta = FieldDescriptor(
    FieldPath{1, 1, 10, 1, 3, 1001, 2},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_band_1_focus_pipeline_base_rf_frequency_fine_delta = FieldDescriptor(
    FieldPath{1, 1, 10, 1, 3, 1001, 3},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_band_1_focus_pipeline_base_rf_frequency_use_fine = FieldDescriptor(
    FieldPath{1, 1, 10, 1, 3, 1001, 4},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_band_1_focus_pipeline_base_rf_max_negative_offset = FieldDescriptor(
    FieldPath{1, 1, 10, 1, 3, 4},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_band_1_focus_pipeline_base_rf_max_positive_offset = FieldDescriptor(
    FieldPath{1, 1, 10, 1, 3, 5},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_band_1_focus_pipeline_base_iq_corrections = FieldDescriptor(
    FieldPath{1, 1, 10, 1, 4},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_band_1_focus_pipeline_base_iq_corrections_amplitude = FieldDescriptor(
    FieldPath{1, 1, 10, 1, 4, 1001},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_band_1_focus_pipeline_base_iq_corrections_amplitude_value = FieldDescriptor(
    FieldPath{1, 1, 10, 1, 4, 1001, 1},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_band_1_focus_pipeline_base_iq_corrections_amplitude_coarse_delta = FieldDescriptor(
    FieldPath{1, 1, 10, 1, 4, 1001, 2},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_band_1_focus_pipeline_base_iq_corrections_amplitude_fine_delta = FieldDescriptor(
    FieldPath{1, 1, 10, 1, 4, 1001, 3},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_band_1_focus_pipeline_base_iq_corrections_amplitude_use_fine = FieldDescriptor(
    FieldPath{1, 1, 10, 1, 4, 1001, 4},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_band_1_focus_pipeline_base_iq_corrections_phase = FieldDescriptor(
    FieldPath{1, 1, 10, 1, 4, 1002},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_band_1_focus_pipeline_base_iq_corrections_phase_value = FieldDescriptor(
    FieldPath{1, 1, 10, 1, 4, 1002, 1},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_band_1_focus_pipeline_base_iq_corrections_phase_coarse_delta = FieldDescriptor(
    FieldPath{1, 1, 10, 1, 4, 1002, 2},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_band_1_focus_pipeline_base_iq_corrections_phase_fine_delta = FieldDescriptor(
    FieldPath{1, 1, 10, 1, 4, 1002, 3},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_band_1_focus_pipeline_base_iq_corrections_phase_use_fine = FieldDescriptor(
    FieldPath{1, 1, 10, 1, 4, 1002, 4},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_band_1_focus_pipeline_mute = FieldDescriptor(
    FieldPath{1, 1, 10, 2},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_band_1_focus_pipeline_agc_speed = FieldDescriptor(
    FieldPath{1, 1, 10, 3},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_band_1_pipeline_a = FieldDescriptor(
    FieldPath{1, 1, 11},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_pipeline_a_base = FieldDescriptor(
    FieldPath{1, 1, 11, 1},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_pipeline_a_base_mode_request = FieldDescriptor(
    FieldPath{1, 1, 11, 1, 1},
    true,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_pipeline_a_base_mode = FieldDescriptor(
    FieldPath{1, 1, 11, 1, 2},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_pipeline_a_base_mode_type = FieldDescriptor(
    FieldPath{1, 1, 11, 1, 2, 1},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_pipeline_a_base_mode_name = FieldDescriptor(
    FieldPath{1, 1, 11, 1, 2, 2},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_pipeline_a_base_mode_label = FieldDescriptor(
    FieldPath{1, 1, 11, 1, 2, 3},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_pipeline_a_base_mode_lo_cut = FieldDescriptor(
    FieldPath{1, 1, 11, 1, 2, 4},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_pipeline_a_base_mode_hi_cut = FieldDescriptor(
    FieldPath{1, 1, 11, 1, 2, 5},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_pipeline_a_base_mode_offset = FieldDescriptor(
    FieldPath{1, 1, 11, 1, 2, 6},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_pipeline_a_base_rf = FieldDescriptor(
    FieldPath{1, 1, 11, 1, 3},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_pipeline_a_base_rf_frequency = FieldDescriptor(
    FieldPath{1, 1, 11, 1, 3, 1001},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_pipeline_a_base_rf_frequency_value = FieldDescriptor(
    FieldPath{1, 1, 11, 1, 3, 1001, 1},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_pipeline_a_base_rf_frequency_coarse_delta = FieldDescriptor(
    FieldPath{1, 1, 11, 1, 3, 1001, 2},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_pipeline_a_base_rf_frequency_fine_delta = FieldDescriptor(
    FieldPath{1, 1, 11, 1, 3, 1001, 3},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_pipeline_a_base_rf_frequency_use_fine = FieldDescriptor(
    FieldPath{1, 1, 11, 1, 3, 1001, 4},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_pipeline_a_base_rf_max_negative_offset = FieldDescriptor(
    FieldPath{1, 1, 11, 1, 3, 4},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_pipeline_a_base_rf_max_positive_offset = FieldDescriptor(
    FieldPath{1, 1, 11, 1, 3, 5},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_pipeline_a_base_iq_corrections = FieldDescriptor(
    FieldPath{1, 1, 11, 1, 4},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_pipeline_a_base_iq_corrections_amplitude = FieldDescriptor(
    FieldPath{1, 1, 11, 1, 4, 1001},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_pipeline_a_base_iq_corrections_amplitude_value = FieldDescriptor(
    FieldPath{1, 1, 11, 1, 4, 1001, 1},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_pipeline_a_base_iq_corrections_amplitude_coarse_delta = FieldDescriptor(
    FieldPath{1, 1, 11, 1, 4, 1001, 2},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_pipeline_a_base_iq_corrections_amplitude_fine_delta = FieldDescriptor(
    FieldPath{1, 1, 11, 1, 4, 1001, 3},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_pipeline_a_base_iq_corrections_amplitude_use_fine = FieldDescriptor(
    FieldPath{1, 1, 11, 1, 4, 1001, 4},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_pipeline_a_base_iq_corrections_phase = FieldDescriptor(
    FieldPath{1, 1, 11, 1, 4, 1002},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_pipeline_a_base_iq_corrections_phase_value = FieldDescriptor(
    FieldPath{1, 1, 11, 1, 4, 1002, 1},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_pipeline_a_base_iq_corrections_phase_coarse_delta = FieldDescriptor(
    FieldPath{1, 1, 11, 1, 4, 1002, 2},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_pipeline_a_base_iq_corrections_phase_fine_delta = FieldDescriptor(
    FieldPath{1, 1, 11, 1, 4, 1002, 3},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_pipeline_a_base_iq_corrections_phase_use_fine = FieldDescriptor(
    FieldPath{1, 1, 11, 1, 4, 1002, 4},
    false,
    false
);
*/
const FieldDescriptor active_bands_band_1_pipeline_a_mute = FieldDescriptor(
    FieldPath{1, 2, 11, 2},
    false,
    false
);
/*
const FieldDescriptor active_bands_band_1_pipeline_a_agc_speed = FieldDescriptor(
    FieldPath{1, 1, 11, 3},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_pipeline_b = FieldDescriptor(
    FieldPath{1, 1, 12},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_pipeline_b_base = FieldDescriptor(
    FieldPath{1, 1, 12, 1},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_pipeline_b_base_mode_request = FieldDescriptor(
    FieldPath{1, 1, 12, 1, 1},
    true,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_pipeline_b_base_mode = FieldDescriptor(
    FieldPath{1, 1, 12, 1, 2},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_pipeline_b_base_mode_type = FieldDescriptor(
    FieldPath{1, 1, 12, 1, 2, 1},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_pipeline_b_base_mode_name = FieldDescriptor(
    FieldPath{1, 1, 12, 1, 2, 2},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_pipeline_b_base_mode_label = FieldDescriptor(
    FieldPath{1, 1, 12, 1, 2, 3},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_pipeline_b_base_mode_lo_cut = FieldDescriptor(
    FieldPath{1, 1, 12, 1, 2, 4},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_pipeline_b_base_mode_hi_cut = FieldDescriptor(
    FieldPath{1, 1, 12, 1, 2, 5},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_pipeline_b_base_mode_offset = FieldDescriptor(
    FieldPath{1, 1, 12, 1, 2, 6},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_pipeline_b_base_rf = FieldDescriptor(
    FieldPath{1, 1, 12, 1, 3},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_pipeline_b_base_rf_frequency = FieldDescriptor(
    FieldPath{1, 1, 12, 1, 3, 1001},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_pipeline_b_base_rf_frequency_value = FieldDescriptor(
    FieldPath{1, 1, 12, 1, 3, 1001, 1},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_pipeline_b_base_rf_frequency_coarse_delta = FieldDescriptor(
    FieldPath{1, 1, 12, 1, 3, 1001, 2},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_pipeline_b_base_rf_frequency_fine_delta = FieldDescriptor(
    FieldPath{1, 1, 12, 1, 3, 1001, 3},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_pipeline_b_base_rf_frequency_use_fine = FieldDescriptor(
    FieldPath{1, 1, 12, 1, 3, 1001, 4},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_pipeline_b_base_rf_max_negative_offset = FieldDescriptor(
    FieldPath{1, 1, 12, 1, 3, 4},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_pipeline_b_base_rf_max_positive_offset = FieldDescriptor(
    FieldPath{1, 1, 12, 1, 3, 5},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_pipeline_b_base_iq_corrections = FieldDescriptor(
    FieldPath{1, 1, 12, 1, 4},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_pipeline_b_base_iq_corrections_amplitude = FieldDescriptor(
    FieldPath{1, 1, 12, 1, 4, 1001},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_pipeline_b_base_iq_corrections_amplitude_value = FieldDescriptor(
    FieldPath{1, 1, 12, 1, 4, 1001, 1},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_pipeline_b_base_iq_corrections_amplitude_coarse_delta = FieldDescriptor(
    FieldPath{1, 1, 12, 1, 4, 1001, 2},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_pipeline_b_base_iq_corrections_amplitude_fine_delta = FieldDescriptor(
    FieldPath{1, 1, 12, 1, 4, 1001, 3},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_pipeline_b_base_iq_corrections_amplitude_use_fine = FieldDescriptor(
    FieldPath{1, 1, 12, 1, 4, 1001, 4},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_pipeline_b_base_iq_corrections_phase = FieldDescriptor(
    FieldPath{1, 1, 12, 1, 4, 1002},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_pipeline_b_base_iq_corrections_phase_value = FieldDescriptor(
    FieldPath{1, 1, 12, 1, 4, 1002, 1},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_pipeline_b_base_iq_corrections_phase_coarse_delta = FieldDescriptor(
    FieldPath{1, 1, 12, 1, 4, 1002, 2},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_pipeline_b_base_iq_corrections_phase_fine_delta = FieldDescriptor(
    FieldPath{1, 1, 12, 1, 4, 1002, 3},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_pipeline_b_base_iq_corrections_phase_use_fine = FieldDescriptor(
    FieldPath{1, 1, 12, 1, 4, 1002, 4},
    false,
    false
);
*/
const FieldDescriptor active_bands_band_1_pipeline_b_mute = FieldDescriptor(
    FieldPath{1, 2, 12, 2},
    false,
    false
);
/*
const FieldDescriptor active_bands_band_1_pipeline_b_agc_speed = FieldDescriptor(
    FieldPath{1, 2, 12, 3},
    false,
    false
);
*/
const FieldDescriptor active_bands_band_1_is_multi_pipeline = FieldDescriptor(
    FieldPath{1, 2, 13},
    true,
    false
);
const FieldDescriptor active_bands_band_1_focus_pipeline_id = FieldDescriptor(
    FieldPath{1, 2, 14},
    false,
    false
);
/*
const FieldDescriptor active_bands_band_1_tx_pipeline = FieldDescriptor(
    FieldPath{1, 1, 15},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_tx_pipeline_base = FieldDescriptor(
    FieldPath{1, 1, 15, 1},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_tx_pipeline_base_mode_request = FieldDescriptor(
    FieldPath{1, 1, 15, 1, 1},
    true,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_tx_pipeline_base_mode = FieldDescriptor(
    FieldPath{1, 1, 15, 1, 2},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_tx_pipeline_base_mode_type = FieldDescriptor(
    FieldPath{1, 1, 15, 1, 2, 1},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_tx_pipeline_base_mode_name = FieldDescriptor(
    FieldPath{1, 1, 15, 1, 2, 2},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_tx_pipeline_base_mode_label = FieldDescriptor(
    FieldPath{1, 1, 15, 1, 2, 3},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_tx_pipeline_base_mode_lo_cut = FieldDescriptor(
    FieldPath{1, 1, 15, 1, 2, 4},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_tx_pipeline_base_mode_hi_cut = FieldDescriptor(
    FieldPath{1, 1, 15, 1, 2, 5},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_tx_pipeline_base_mode_offset = FieldDescriptor(
    FieldPath{1, 1, 15, 1, 2, 6},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_tx_pipeline_base_rf = FieldDescriptor(
    FieldPath{1, 1, 15, 1, 3},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_tx_pipeline_base_rf_frequency = FieldDescriptor(
    FieldPath{1, 1, 15, 1, 3, 1001},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_tx_pipeline_base_rf_frequency_value = FieldDescriptor(
    FieldPath{1, 1, 15, 1, 3, 1001, 1},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_tx_pipeline_base_rf_frequency_coarse_delta = FieldDescriptor(
    FieldPath{1, 1, 15, 1, 3, 1001, 2},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_tx_pipeline_base_rf_frequency_fine_delta = FieldDescriptor(
    FieldPath{1, 1, 15, 1, 3, 1001, 3},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_tx_pipeline_base_rf_frequency_use_fine = FieldDescriptor(
    FieldPath{1, 1, 15, 1, 3, 1001, 4},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_tx_pipeline_base_rf_max_negative_offset = FieldDescriptor(
    FieldPath{1, 1, 15, 1, 3, 4},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_tx_pipeline_base_rf_max_positive_offset = FieldDescriptor(
    FieldPath{1, 1, 15, 1, 3, 5},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_tx_pipeline_base_iq_corrections = FieldDescriptor(
    FieldPath{1, 1, 15, 1, 4},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_tx_pipeline_base_iq_corrections_amplitude = FieldDescriptor(
    FieldPath{1, 1, 15, 1, 4, 1001},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_tx_pipeline_base_iq_corrections_amplitude_value = FieldDescriptor(
    FieldPath{1, 1, 15, 1, 4, 1001, 1},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_tx_pipeline_base_iq_corrections_amplitude_coarse_delta = FieldDescriptor(
    FieldPath{1, 1, 15, 1, 4, 1001, 2},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_tx_pipeline_base_iq_corrections_amplitude_fine_delta = FieldDescriptor(
    FieldPath{1, 1, 15, 1, 4, 1001, 3},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_tx_pipeline_base_iq_corrections_amplitude_use_fine = FieldDescriptor(
    FieldPath{1, 1, 15, 1, 4, 1001, 4},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_tx_pipeline_base_iq_corrections_phase = FieldDescriptor(
    FieldPath{1, 1, 15, 1, 4, 1002},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_tx_pipeline_base_iq_corrections_phase_value = FieldDescriptor(
    FieldPath{1, 1, 15, 1, 4, 1002, 1},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_tx_pipeline_base_iq_corrections_phase_coarse_delta = FieldDescriptor(
    FieldPath{1, 1, 15, 1, 4, 1002, 2},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_tx_pipeline_base_iq_corrections_phase_fine_delta = FieldDescriptor(
    FieldPath{1, 1, 15, 1, 4, 1002, 3},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_1_tx_pipeline_base_iq_corrections_phase_use_fine = FieldDescriptor(
    FieldPath{1, 1, 15, 1, 4, 1002, 4},
    false,
    false
);
*/
const FieldDescriptor active_bands_band_1_tx_pipeline_id = FieldDescriptor(
    FieldPath{1, 2, 16},
    false,
    false
);
/*
const FieldDescriptor active_bands_band_2 = FieldDescriptor(
    FieldPath{1, 2},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_band_request = FieldDescriptor(
    FieldPath{1, 2, 1},
    true,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_band = FieldDescriptor(
    FieldPath{1, 2, 2},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_band_name = FieldDescriptor(
    FieldPath{1, 2, 2, 1},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_band_label = FieldDescriptor(
    FieldPath{1, 2, 2, 2},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_band_lowest_frequency = FieldDescriptor(
    FieldPath{1, 2, 2, 3},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_band_highest_frequency = FieldDescriptor(
    FieldPath{1, 2, 2, 4},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_band_landing_frequency = FieldDescriptor(
    FieldPath{1, 2, 2, 5},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_band_default_fine_step = FieldDescriptor(
    FieldPath{1, 2, 2, 6},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_band_default_coarse_step = FieldDescriptor(
    FieldPath{1, 2, 2, 7},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_band_default_mode = FieldDescriptor(
    FieldPath{1, 2, 2, 8},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_band_default_mode_type = FieldDescriptor(
    FieldPath{1, 2, 2, 8, 1},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_band_default_mode_name = FieldDescriptor(
    FieldPath{1, 2, 2, 8, 2},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_band_default_mode_label = FieldDescriptor(
    FieldPath{1, 2, 2, 8, 3},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_band_default_mode_lo_cut = FieldDescriptor(
    FieldPath{1, 2, 2, 8, 4},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_band_default_mode_hi_cut = FieldDescriptor(
    FieldPath{1, 2, 2, 8, 5},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_band_default_mode_offset = FieldDescriptor(
    FieldPath{1, 2, 2, 8, 6},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_rf = FieldDescriptor(
    FieldPath{1, 2, 5},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_rf_frequency = FieldDescriptor(
    FieldPath{1, 2, 5, 1001},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_rf_frequency_value = FieldDescriptor(
    FieldPath{1, 2, 5, 1001, 1},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_rf_frequency_coarse_delta = FieldDescriptor(
    FieldPath{1, 2, 5, 1001, 2},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_rf_frequency_fine_delta = FieldDescriptor(
    FieldPath{1, 2, 5, 1001, 3},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_rf_frequency_use_fine = FieldDescriptor(
    FieldPath{1, 2, 5, 1001, 4},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_rf_gain = FieldDescriptor(
    FieldPath{1, 2, 5, 1002},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_rf_gain_value = FieldDescriptor(
    FieldPath{1, 2, 5, 1002, 1},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_rf_gain_coarse_delta = FieldDescriptor(
    FieldPath{1, 2, 5, 1002, 2},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_rf_gain_fine_delta = FieldDescriptor(
    FieldPath{1, 2, 5, 1002, 3},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_rf_gain_use_fine = FieldDescriptor(
    FieldPath{1, 2, 5, 1002, 4},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_if = FieldDescriptor(
    FieldPath{1, 2, 6},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_if_gain = FieldDescriptor(
    FieldPath{1, 2, 6, 1001},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_if_gain_value = FieldDescriptor(
    FieldPath{1, 2, 6, 1001, 1},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_if_gain_coarse_delta = FieldDescriptor(
    FieldPath{1, 2, 6, 1001, 2},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_if_gain_fine_delta = FieldDescriptor(
    FieldPath{1, 2, 6, 1001, 3},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_if_gain_use_fine = FieldDescriptor(
    FieldPath{1, 2, 6, 1001, 4},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_if_bandwidth = FieldDescriptor(
    FieldPath{1, 2, 6, 2},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_focus_pipeline = FieldDescriptor(
    FieldPath{1, 2, 10},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_band_2_focus_pipeline_base = FieldDescriptor(
    FieldPath{1, 2, 10, 1},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_band_2_focus_pipeline_base_mode_request = FieldDescriptor(
    FieldPath{1, 2, 10, 1, 1},
    true,
    true
);
*/
/*
const FieldDescriptor active_bands_band_2_focus_pipeline_base_mode = FieldDescriptor(
    FieldPath{1, 2, 10, 1, 2},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_band_2_focus_pipeline_base_mode_type = FieldDescriptor(
    FieldPath{1, 2, 10, 1, 2, 1},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_band_2_focus_pipeline_base_mode_name = FieldDescriptor(
    FieldPath{1, 2, 10, 1, 2, 2},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_band_2_focus_pipeline_base_mode_label = FieldDescriptor(
    FieldPath{1, 2, 10, 1, 2, 3},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_band_2_focus_pipeline_base_mode_lo_cut = FieldDescriptor(
    FieldPath{1, 2, 10, 1, 2, 4},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_band_2_focus_pipeline_base_mode_hi_cut = FieldDescriptor(
    FieldPath{1, 2, 10, 1, 2, 5},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_band_2_focus_pipeline_base_mode_offset = FieldDescriptor(
    FieldPath{1, 2, 10, 1, 2, 6},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_band_2_focus_pipeline_base_rf = FieldDescriptor(
    FieldPath{1, 2, 10, 1, 3},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_band_2_focus_pipeline_base_rf_frequency = FieldDescriptor(
    FieldPath{1, 2, 10, 1, 3, 1001},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_band_2_focus_pipeline_base_rf_frequency_value = FieldDescriptor(
    FieldPath{1, 2, 10, 1, 3, 1001, 1},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_band_2_focus_pipeline_base_rf_frequency_coarse_delta = FieldDescriptor(
    FieldPath{1, 2, 10, 1, 3, 1001, 2},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_band_2_focus_pipeline_base_rf_frequency_fine_delta = FieldDescriptor(
    FieldPath{1, 2, 10, 1, 3, 1001, 3},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_band_2_focus_pipeline_base_rf_frequency_use_fine = FieldDescriptor(
    FieldPath{1, 2, 10, 1, 3, 1001, 4},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_band_2_focus_pipeline_base_rf_max_negative_offset = FieldDescriptor(
    FieldPath{1, 2, 10, 1, 3, 4},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_band_2_focus_pipeline_base_rf_max_positive_offset = FieldDescriptor(
    FieldPath{1, 2, 10, 1, 3, 5},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_band_2_focus_pipeline_base_iq_corrections = FieldDescriptor(
    FieldPath{1, 2, 10, 1, 4},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_band_2_focus_pipeline_base_iq_corrections_amplitude = FieldDescriptor(
    FieldPath{1, 2, 10, 1, 4, 1001},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_band_2_focus_pipeline_base_iq_corrections_amplitude_value = FieldDescriptor(
    FieldPath{1, 2, 10, 1, 4, 1001, 1},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_band_2_focus_pipeline_base_iq_corrections_amplitude_coarse_delta = FieldDescriptor(
    FieldPath{1, 2, 10, 1, 4, 1001, 2},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_band_2_focus_pipeline_base_iq_corrections_amplitude_fine_delta = FieldDescriptor(
    FieldPath{1, 2, 10, 1, 4, 1001, 3},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_band_2_focus_pipeline_base_iq_corrections_amplitude_use_fine = FieldDescriptor(
    FieldPath{1, 2, 10, 1, 4, 1001, 4},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_band_2_focus_pipeline_base_iq_corrections_phase = FieldDescriptor(
    FieldPath{1, 2, 10, 1, 4, 1002},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_band_2_focus_pipeline_base_iq_corrections_phase_value = FieldDescriptor(
    FieldPath{1, 2, 10, 1, 4, 1002, 1},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_band_2_focus_pipeline_base_iq_corrections_phase_coarse_delta = FieldDescriptor(
    FieldPath{1, 2, 10, 1, 4, 1002, 2},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_band_2_focus_pipeline_base_iq_corrections_phase_fine_delta = FieldDescriptor(
    FieldPath{1, 2, 10, 1, 4, 1002, 3},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_band_2_focus_pipeline_base_iq_corrections_phase_use_fine = FieldDescriptor(
    FieldPath{1, 2, 10, 1, 4, 1002, 4},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_band_2_focus_pipeline_mute = FieldDescriptor(
    FieldPath{1, 2, 10, 2},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_band_2_focus_pipeline_agc_speed = FieldDescriptor(
    FieldPath{1, 2, 10, 3},
    false,
    true
);
*/
/*
const FieldDescriptor active_bands_band_2_pipeline_a = FieldDescriptor(
    FieldPath{1, 2, 11},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_pipeline_a_base = FieldDescriptor(
    FieldPath{1, 2, 11, 1},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_pipeline_a_base_mode_request = FieldDescriptor(
    FieldPath{1, 2, 11, 1, 1},
    true,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_pipeline_a_base_mode = FieldDescriptor(
    FieldPath{1, 2, 11, 1, 2},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_pipeline_a_base_mode_type = FieldDescriptor(
    FieldPath{1, 2, 11, 1, 2, 1},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_pipeline_a_base_mode_name = FieldDescriptor(
    FieldPath{1, 2, 11, 1, 2, 2},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_pipeline_a_base_mode_label = FieldDescriptor(
    FieldPath{1, 2, 11, 1, 2, 3},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_pipeline_a_base_mode_lo_cut = FieldDescriptor(
    FieldPath{1, 2, 11, 1, 2, 4},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_pipeline_a_base_mode_hi_cut = FieldDescriptor(
    FieldPath{1, 2, 11, 1, 2, 5},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_pipeline_a_base_mode_offset = FieldDescriptor(
    FieldPath{1, 2, 11, 1, 2, 6},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_pipeline_a_base_rf = FieldDescriptor(
    FieldPath{1, 2, 11, 1, 3},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_pipeline_a_base_rf_frequency = FieldDescriptor(
    FieldPath{1, 2, 11, 1, 3, 1001},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_pipeline_a_base_rf_frequency_value = FieldDescriptor(
    FieldPath{1, 2, 11, 1, 3, 1001, 1},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_pipeline_a_base_rf_frequency_coarse_delta = FieldDescriptor(
    FieldPath{1, 2, 11, 1, 3, 1001, 2},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_pipeline_a_base_rf_frequency_fine_delta = FieldDescriptor(
    FieldPath{1, 2, 11, 1, 3, 1001, 3},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_pipeline_a_base_rf_frequency_use_fine = FieldDescriptor(
    FieldPath{1, 2, 11, 1, 3, 1001, 4},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_pipeline_a_base_rf_max_negative_offset = FieldDescriptor(
    FieldPath{1, 2, 11, 1, 3, 4},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_pipeline_a_base_rf_max_positive_offset = FieldDescriptor(
    FieldPath{1, 2, 11, 1, 3, 5},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_pipeline_a_base_iq_corrections = FieldDescriptor(
    FieldPath{1, 2, 11, 1, 4},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_pipeline_a_base_iq_corrections_amplitude = FieldDescriptor(
    FieldPath{1, 2, 11, 1, 4, 1001},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_pipeline_a_base_iq_corrections_amplitude_value = FieldDescriptor(
    FieldPath{1, 2, 11, 1, 4, 1001, 1},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_pipeline_a_base_iq_corrections_amplitude_coarse_delta = FieldDescriptor(
    FieldPath{1, 2, 11, 1, 4, 1001, 2},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_pipeline_a_base_iq_corrections_amplitude_fine_delta = FieldDescriptor(
    FieldPath{1, 2, 11, 1, 4, 1001, 3},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_pipeline_a_base_iq_corrections_amplitude_use_fine = FieldDescriptor(
    FieldPath{1, 2, 11, 1, 4, 1001, 4},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_pipeline_a_base_iq_corrections_phase = FieldDescriptor(
    FieldPath{1, 2, 11, 1, 4, 1002},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_pipeline_a_base_iq_corrections_phase_value = FieldDescriptor(
    FieldPath{1, 2, 11, 1, 4, 1002, 1},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_pipeline_a_base_iq_corrections_phase_coarse_delta = FieldDescriptor(
    FieldPath{1, 2, 11, 1, 4, 1002, 2},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_pipeline_a_base_iq_corrections_phase_fine_delta = FieldDescriptor(
    FieldPath{1, 2, 11, 1, 4, 1002, 3},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_pipeline_a_base_iq_corrections_phase_use_fine = FieldDescriptor(
    FieldPath{1, 2, 11, 1, 4, 1002, 4},
    false,
    false
);
*/
const FieldDescriptor active_bands_band_2_pipeline_a_mute = FieldDescriptor(
    FieldPath{1, 3, 11, 2},
    false,
    false
);
/*
const FieldDescriptor active_bands_band_2_pipeline_a_agc_speed = FieldDescriptor(
    FieldPath{1, 2, 11, 3},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_pipeline_b = FieldDescriptor(
    FieldPath{1, 2, 12},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_pipeline_b_base = FieldDescriptor(
    FieldPath{1, 2, 12, 1},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_pipeline_b_base_mode_request = FieldDescriptor(
    FieldPath{1, 2, 12, 1, 1},
    true,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_pipeline_b_base_mode = FieldDescriptor(
    FieldPath{1, 2, 12, 1, 2},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_pipeline_b_base_mode_type = FieldDescriptor(
    FieldPath{1, 2, 12, 1, 2, 1},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_pipeline_b_base_mode_name = FieldDescriptor(
    FieldPath{1, 2, 12, 1, 2, 2},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_pipeline_b_base_mode_label = FieldDescriptor(
    FieldPath{1, 2, 12, 1, 2, 3},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_pipeline_b_base_mode_lo_cut = FieldDescriptor(
    FieldPath{1, 2, 12, 1, 2, 4},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_pipeline_b_base_mode_hi_cut = FieldDescriptor(
    FieldPath{1, 2, 12, 1, 2, 5},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_pipeline_b_base_mode_offset = FieldDescriptor(
    FieldPath{1, 2, 12, 1, 2, 6},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_pipeline_b_base_rf = FieldDescriptor(
    FieldPath{1, 2, 12, 1, 3},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_pipeline_b_base_rf_frequency = FieldDescriptor(
    FieldPath{1, 2, 12, 1, 3, 1001},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_pipeline_b_base_rf_frequency_value = FieldDescriptor(
    FieldPath{1, 2, 12, 1, 3, 1001, 1},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_pipeline_b_base_rf_frequency_coarse_delta = FieldDescriptor(
    FieldPath{1, 2, 12, 1, 3, 1001, 2},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_pipeline_b_base_rf_frequency_fine_delta = FieldDescriptor(
    FieldPath{1, 2, 12, 1, 3, 1001, 3},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_pipeline_b_base_rf_frequency_use_fine = FieldDescriptor(
    FieldPath{1, 2, 12, 1, 3, 1001, 4},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_pipeline_b_base_rf_max_negative_offset = FieldDescriptor(
    FieldPath{1, 2, 12, 1, 3, 4},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_pipeline_b_base_rf_max_positive_offset = FieldDescriptor(
    FieldPath{1, 2, 12, 1, 3, 5},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_pipeline_b_base_iq_corrections = FieldDescriptor(
    FieldPath{1, 2, 12, 1, 4},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_pipeline_b_base_iq_corrections_amplitude = FieldDescriptor(
    FieldPath{1, 2, 12, 1, 4, 1001},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_pipeline_b_base_iq_corrections_amplitude_value = FieldDescriptor(
    FieldPath{1, 2, 12, 1, 4, 1001, 1},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_pipeline_b_base_iq_corrections_amplitude_coarse_delta = FieldDescriptor(
    FieldPath{1, 2, 12, 1, 4, 1001, 2},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_pipeline_b_base_iq_corrections_amplitude_fine_delta = FieldDescriptor(
    FieldPath{1, 2, 12, 1, 4, 1001, 3},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_pipeline_b_base_iq_corrections_amplitude_use_fine = FieldDescriptor(
    FieldPath{1, 2, 12, 1, 4, 1001, 4},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_pipeline_b_base_iq_corrections_phase = FieldDescriptor(
    FieldPath{1, 2, 12, 1, 4, 1002},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_pipeline_b_base_iq_corrections_phase_value = FieldDescriptor(
    FieldPath{1, 2, 12, 1, 4, 1002, 1},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_pipeline_b_base_iq_corrections_phase_coarse_delta = FieldDescriptor(
    FieldPath{1, 2, 12, 1, 4, 1002, 2},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_pipeline_b_base_iq_corrections_phase_fine_delta = FieldDescriptor(
    FieldPath{1, 2, 12, 1, 4, 1002, 3},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_pipeline_b_base_iq_corrections_phase_use_fine = FieldDescriptor(
    FieldPath{1, 2, 12, 1, 4, 1002, 4},
    false,
    false
);
*/
const FieldDescriptor active_bands_band_2_pipeline_b_mute = FieldDescriptor(
    FieldPath{1, 3, 12, 2},
    false,
    false
);
/*
const FieldDescriptor active_bands_band_2_pipeline_b_agc_speed = FieldDescriptor(
    FieldPath{1, 2, 12, 3},
    false,
    false
);
*/
const FieldDescriptor active_bands_band_2_is_multi_pipeline = FieldDescriptor(
    FieldPath{1, 3, 13},
    true,
    false
);
const FieldDescriptor active_bands_band_2_focus_pipeline_id = FieldDescriptor(
    FieldPath{1, 3, 14},
    false,
    false
);
/*
const FieldDescriptor active_bands_band_2_tx_pipeline = FieldDescriptor(
    FieldPath{1, 2, 15},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_tx_pipeline_base = FieldDescriptor(
    FieldPath{1, 2, 15, 1},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_tx_pipeline_base_mode_request = FieldDescriptor(
    FieldPath{1, 2, 15, 1, 1},
    true,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_tx_pipeline_base_mode = FieldDescriptor(
    FieldPath{1, 2, 15, 1, 2},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_tx_pipeline_base_mode_type = FieldDescriptor(
    FieldPath{1, 2, 15, 1, 2, 1},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_tx_pipeline_base_mode_name = FieldDescriptor(
    FieldPath{1, 2, 15, 1, 2, 2},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_tx_pipeline_base_mode_label = FieldDescriptor(
    FieldPath{1, 2, 15, 1, 2, 3},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_tx_pipeline_base_mode_lo_cut = FieldDescriptor(
    FieldPath{1, 2, 15, 1, 2, 4},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_tx_pipeline_base_mode_hi_cut = FieldDescriptor(
    FieldPath{1, 2, 15, 1, 2, 5},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_tx_pipeline_base_mode_offset = FieldDescriptor(
    FieldPath{1, 2, 15, 1, 2, 6},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_tx_pipeline_base_rf = FieldDescriptor(
    FieldPath{1, 2, 15, 1, 3},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_tx_pipeline_base_rf_frequency = FieldDescriptor(
    FieldPath{1, 2, 15, 1, 3, 1001},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_tx_pipeline_base_rf_frequency_value = FieldDescriptor(
    FieldPath{1, 2, 15, 1, 3, 1001, 1},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_tx_pipeline_base_rf_frequency_coarse_delta = FieldDescriptor(
    FieldPath{1, 2, 15, 1, 3, 1001, 2},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_tx_pipeline_base_rf_frequency_fine_delta = FieldDescriptor(
    FieldPath{1, 2, 15, 1, 3, 1001, 3},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_tx_pipeline_base_rf_frequency_use_fine = FieldDescriptor(
    FieldPath{1, 2, 15, 1, 3, 1001, 4},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_tx_pipeline_base_rf_max_negative_offset = FieldDescriptor(
    FieldPath{1, 2, 15, 1, 3, 4},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_tx_pipeline_base_rf_max_positive_offset = FieldDescriptor(
    FieldPath{1, 2, 15, 1, 3, 5},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_tx_pipeline_base_iq_corrections = FieldDescriptor(
    FieldPath{1, 2, 15, 1, 4},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_tx_pipeline_base_iq_corrections_amplitude = FieldDescriptor(
    FieldPath{1, 2, 15, 1, 4, 1001},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_tx_pipeline_base_iq_corrections_amplitude_value = FieldDescriptor(
    FieldPath{1, 2, 15, 1, 4, 1001, 1},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_tx_pipeline_base_iq_corrections_amplitude_coarse_delta = FieldDescriptor(
    FieldPath{1, 2, 15, 1, 4, 1001, 2},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_tx_pipeline_base_iq_corrections_amplitude_fine_delta = FieldDescriptor(
    FieldPath{1, 2, 15, 1, 4, 1001, 3},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_tx_pipeline_base_iq_corrections_amplitude_use_fine = FieldDescriptor(
    FieldPath{1, 2, 15, 1, 4, 1001, 4},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_tx_pipeline_base_iq_corrections_phase = FieldDescriptor(
    FieldPath{1, 2, 15, 1, 4, 1002},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_tx_pipeline_base_iq_corrections_phase_value = FieldDescriptor(
    FieldPath{1, 2, 15, 1, 4, 1002, 1},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_tx_pipeline_base_iq_corrections_phase_coarse_delta = FieldDescriptor(
    FieldPath{1, 2, 15, 1, 4, 1002, 2},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_tx_pipeline_base_iq_corrections_phase_fine_delta = FieldDescriptor(
    FieldPath{1, 2, 15, 1, 4, 1002, 3},
    false,
    false
);
*/
/*
const FieldDescriptor active_bands_band_2_tx_pipeline_base_iq_corrections_phase_use_fine = FieldDescriptor(
    FieldPath{1, 2, 15, 1, 4, 1002, 4},
    false,
    false
);
*/
const FieldDescriptor active_bands_band_2_tx_pipeline_id = FieldDescriptor(
    FieldPath{1, 3, 16},
    false,
    false
);
const FieldDescriptor active_bands_focus_band_id = FieldDescriptor(
    FieldPath{1, 4},
    false,
    false
);
const FieldDescriptor active_bands_tx_band_id = FieldDescriptor(
    FieldPath{1, 5},
    false,
    false
);
/*
const FieldDescriptor active_bands_rx_band_id = FieldDescriptor(
    FieldPath{1, 6},
    false,
    false
);
*/
const FieldDescriptor active_bands_is_split = FieldDescriptor(
    FieldPath{1, 7},
    true,
    false
);
/*
const FieldDescriptor receiver = FieldDescriptor(
    FieldPath{2},
    false,
    false
);
*/
/*
const FieldDescriptor receiver_volume = FieldDescriptor(
    FieldPath{2, 2},
    false,
    false
);
*/
/*
const FieldDescriptor receiver_volume_gain = FieldDescriptor(
    FieldPath{2, 2, 1001},
    false,
    false
);
*/
/*
const FieldDescriptor receiver_volume_gain_value = FieldDescriptor(
    FieldPath{2, 2, 1001, 1},
    false,
    false
);
*/
/*
const FieldDescriptor receiver_volume_gain_coarse_delta = FieldDescriptor(
    FieldPath{2, 2, 1001, 2},
    false,
    false
);
*/
/*
const FieldDescriptor receiver_volume_gain_fine_delta = FieldDescriptor(
    FieldPath{2, 2, 1001, 3},
    false,
    false
);
*/
/*
const FieldDescriptor receiver_volume_gain_use_fine = FieldDescriptor(
    FieldPath{2, 2, 1001, 4},
    false,
    false
);
*/
/*
const FieldDescriptor transmitter = FieldDescriptor(
    FieldPath{3},
    false,
    false
);
*/
/*
const FieldDescriptor transmitter_mic = FieldDescriptor(
    FieldPath{3, 2},
    false,
    false
);
*/
/*
const FieldDescriptor transmitter_mic_gain = FieldDescriptor(
    FieldPath{3, 2, 1001},
    false,
    false
);
*/
/*
const FieldDescriptor transmitter_mic_gain_value = FieldDescriptor(
    FieldPath{3, 2, 1001, 1},
    false,
    false
);
*/
/*
const FieldDescriptor transmitter_mic_gain_coarse_delta = FieldDescriptor(
    FieldPath{3, 2, 1001, 2},
    false,
    false
);
*/
/*
const FieldDescriptor transmitter_mic_gain_fine_delta = FieldDescriptor(
    FieldPath{3, 2, 1001, 3},
    false,
    false
);
*/
/*
const FieldDescriptor transmitter_mic_gain_use_fine = FieldDescriptor(
    FieldPath{3, 2, 1001, 4},
    false,
    false
);
*/
const FieldDescriptor ptt = FieldDescriptor(
    FieldPath{4},
    false,
    false
);
