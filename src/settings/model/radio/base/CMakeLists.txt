cmake_minimum_required(VERSION 4.1)

project(settings-model-radio-base)

set(RADIOS_BASE_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radio/RadioSettingsBaseT.h

        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radio/AfSettings.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radio/AgcSpeed.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radio/Band.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radio/BandRfSettings.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radio/BandSettingsCacheT.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radio/Capabilities.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radio/CapabilitiesMessage.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radio/ActiveBandSettingsDefaultsT.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radio/BandSettingsDefaults.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radio/IfSettings.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radio/RadioSettingsTraits.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radio/IRadioSettingsMirror.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radio/IRadioSettingsUpdater.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radio/Mode.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radio/PipelineId.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radio/RadioSettingsEventT.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radio/RadioSettingsMirrorT.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radio/RadioRequester.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radio/RadioSettingsSinkT.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radio/RadioSettingsSourceT.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radio/RadioSettingsUpdater.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radio/ReceiverSettings.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radio/RxMeteringMessage.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radio/SplitBandId.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radio/StringRef.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radio/TransmitterSettings.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radio/WithBandT.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radio/WithModeT.h
)

add_library(settings-model-radio-base INTERFACE ${RADIOS_BASE_FILES})

target_include_directories(settings-model-radio-base INTERFACE
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(settings-model-radio-base INTERFACE
        settings-model-proto
        settings-model-message
        settings-model-meta-band
        settings-model-meta-mode
        settings-model-update-base
)