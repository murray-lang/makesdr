cmake_minimum_required(VERSION 4.1)

project(RadioSettingsProto-settings-model-basic)

set(BASIC_SETTINGS_SOURCES
        ${PROTO_SRCS}
        ${PROTO_HDRS}
#        ../../../../include/ResultCode.h
        include/model/basic/StringRef.h
        include/model/basic/ActiveBandSettings.h
        include/model/basic/BasicActiveBandSettings.h
        include/model/basic/BandSettings.h
        include/model/basic/BasicBandSettings.h
        include/model/basic/SplitBandId.h
        include/model/basic/Band.h
        include/model/basic/SettingsBase.h
        include/model/basic/ResolveIndirection.h

        ../core/include/model/core/SettingFieldPath.h
        ../core/include/model/core/SettingFieldVariant.h

        include/model/basic/SteppableInt64Setting.h
        include/model/basic/SteppableFloatSetting.h
        include/model/basic/SteppableVariant.h
        include/model/basic/Mode.h
        include/model/basic/PipelineSettings.h
        include/model/basic/IqCorrectionSettings.h
        include/model/basic/MicrophoneSettings.h
        include/model/basic/PipelineId.h
        include/model/basic/AgcSpeed.h
        include/model/basic/RfSettings.h
        include/model/basic/IfSettings.h
        include/model/basic/RxPipelineSettings.h
        include/model/basic/BasicRxPipelineSettings.h
        include/model/basic/TxPipelineSettings.h
        include/model/basic/BasicTxPipelineSettings.h
        include/model/basic/RadioSettings.h
        include/model/basic/BasicRadioSettings.h
        include/model/basic/BasicRadioSettings.h
        include/model/basic/BasicRadioSettingsSink.h
        include/model/basic/ReceiverSettings.h
        include/model/basic/TransmitterSettings.h

        ${CMAKE_CURRENT_LIST_DIR}/src/PipelineSettings.cpp
)

add_library(RadioSettingsProto-settings-model-basic STATIC ${BASIC_SETTINGS_SOURCES})

target_include_directories(RadioSettingsProto-settings-model-basic PUBLIC
#        ${ETL_DIR}/include
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(RadioSettingsProto-settings-model-basic PUBLIC
        RadioSettingsProto-settings-model-proto
        RadioSettingsProto-settings-model-core
        etl
)
