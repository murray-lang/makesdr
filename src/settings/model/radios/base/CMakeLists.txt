cmake_minimum_required(VERSION 4.1)

project(settings-model-radios-base)

set(RADIOS_BASE_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radios/RadioSettingsBaseT.h

        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radios/AfSettings.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radios/AgcSpeed.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radios/Band.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radios/BandRfSettings.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radios/BandSettingsCacheT.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radios/IActiveBandSettingsT.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radios/IBandSettings.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radios/IfSettings.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radios/IRadioSettings.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radios/IRadioSettingsEvent.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radios/IRadioSettingsRequester.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radios/IRadioSettingsUpdater.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radios/Mode.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radios/PipelineId.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radios/RadioSettingsEventT.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radios/RadioSettingsRequester.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radios/RadioSettingsSink.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radios/RadioSettingsSinkT.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radios/RadioSettingsSource.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radios/RadioSettingsSourceT.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radios/RadioSettingsUpdater.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radios/IRadioSettingsWithUpdater.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radios/ReceiverSettings.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radios/SplitBandId.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radios/TransmitterSettings.h
)

add_library(settings-model-radios-base INTERFACE ${RADIOS_BASE_FILES})

target_include_directories(settings-model-radios-base INTERFACE
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(settings-model-radios-base INTERFACE
        settings-model-proto
        settings-model-message
        settings-model-data-band
        settings-model-data-mode
)