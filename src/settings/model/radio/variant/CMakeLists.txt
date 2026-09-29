cmake_minimum_required(VERSION 4.1)

project(settings-model-radio-variant)

set(RADIO_SETTINGS_VARIANT_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radio/variant/RadioSettingsVariant.h
        ${CMAKE_CURRENT_LIST_DIR}/src/RadioSettingsVariant.cpp

)

add_library(settings-model-radio-variant STATIC ${RADIO_SETTINGS_VARIANT_FILES})

target_include_directories(settings-model-radio-variant PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(settings-model-radio-variant PUBLIC
        settings-model-radio-basic-rx
        settings-model-radio-basic-rxtx
        settings-model-radio-iq-basic-rx
        settings-model-radio-iq-basic-rxtx
        settings-model-radio-iq-dual-rx
        settings-model-radio-iq-dual-rxtx
        settings-model-radio-iq-dual-split
)