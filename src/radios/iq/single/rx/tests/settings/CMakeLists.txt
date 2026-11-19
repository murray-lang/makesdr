cmake_minimum_required(VERSION 4.1)

project(radios-iq-single-rx-tests-settings)

set (SINGLE_IQ_RX_TESTS_SETTINGS_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/include/test/settings/testRadioSettings.h
        ${CMAKE_CURRENT_LIST_DIR}/src/testRadioSettings.cpp
)

add_library(radios-iq-single-rx-tests-settings STATIC EXCLUDE_FROM_ALL ${SINGLE_IQ_RX_TESTS_SETTINGS_SOURCES})

target_include_directories(radios-iq-single-rx-tests-settings PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(radios-iq-single-rx-tests-settings PUBLIC
        settings-model-proto
        settings-model-radio-iq-single-rx
        settings-model-meta-band
        settings-model-meta-mode
)
