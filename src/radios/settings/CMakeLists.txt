cmake_minimum_required(VERSION 4.1)

project(radios-settings)

set(RADIO_SETTINGS_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/radios/settings/RadioSettingsVariant.h
)

add_library(radios-settings INTERFACE ${RADIO_SETTINGS_FILES})

target_include_directories(radios-settings INTERFACE
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(radios-settings INTERFACE
        radios-iq-dual-rx-settings-model
        radios-iq-dual-rxtx-settings-model
        radios-iq-dual-split-settings-model
)