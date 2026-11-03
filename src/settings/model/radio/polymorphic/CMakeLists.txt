cmake_minimum_required(VERSION 4.1)

project(settings-model-radio-polymorphic)

set(RADIOS_POLYMORPHIC_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radio/ActiveBandSettingsViewT.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radio/BandSettingsViewT.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radio/IActiveBandSettingsView.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radio/IBandSettingsView.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radio/IRadioSettingsMirror.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radio/IRadioSettingsView.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radio/RadioSettingsMirrorViewT.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radio/RadioSettingsViewT.h

)

add_library(settings-model-radio-polymorphic INTERFACE ${RADIOS_POLYMORPHIC_FILES})

target_include_directories(settings-model-radio-polymorphic INTERFACE
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(settings-model-radio-polymorphic INTERFACE
        settings-model-proto
        settings-model-message
        settings-model-meta-band
        settings-model-meta-mode
        settings-model-update-base
        settings-model-radio-base
)