cmake_minimum_required(VERSION 4.1)

project(settings-model-radios-variant)

set(RADIO_SETTINGS_VARIANT_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radios/variant/RadioSettingsVariant.h
        ${CMAKE_CURRENT_LIST_DIR}/src/RadioSettingsVariant.cpp

)

add_library(settings-model-radios-variant STATIC ${RADIO_SETTINGS_VARIANT_FILES})

target_include_directories(settings-model-radios-variant PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(settings-model-radios-variant PUBLIC
        settings-model-radios-basic-rx
        settings-model-radios-basic-rxtx
        settings-model-radios-iq-basic-rx
        settings-model-radios-iq-basic-rxtx
        settings-model-radios-iq-dual-rx
        settings-model-radios-iq-dual-rxtx
        settings-model-radios-iq-dual-split
)