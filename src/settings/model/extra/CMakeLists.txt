cmake_minimum_required(VERSION 4.1)

project(settings-model-extra)

set(BASIC_SETTINGS_SOURCES
        ${PROTO_SRCS}
        ${PROTO_HDRS}
        ../core/include/settings/model/core/StringRef.h
        ../meta/include/settings/model/meta/RadioMeta.h
        include/settings/model/extra/ActiveBandSettings.h
        include/settings/model/extra/BandSettings.h
        include/settings/model/extra/BandSettings.h

        include/settings/model/extra/Band.h
        include/settings/model/extra/BandCategory.h
        include/settings/model/extra/Mode.h

        include/settings/model/extra/SteppableInt64Setting.h
        include/settings/model/extra/SteppableFloatSetting.h
        include/settings/model/extra/SteppableVariant.h
        include/settings/model/extra/PipelineSettings.h
        include/settings/model/extra/IqCorrectionSettings.h
        include/settings/model/extra/MicrophoneSettings.h
        include/settings/model/extra/AgcSpeed.h
        include/settings/model/extra/RfSettings.h
        include/settings/model/extra/IfSettings.h
        include/settings/model/extra/RxPipelineSettings.h
        include/settings/model/extra/RxPipelineSettings.h
        include/settings/model/extra/TxPipelineSettings.h
        include/settings/model/extra/TxPipelineSettings.h
        include/settings/model/extra/RadioSettingsEx.h
        include/settings/model/extra/ReceiverSettings.h
        include/settings/model/extra/TransmitterSettings.h

        src/ActiveBandSettings.cpp
        src/BandSettings.cpp
        ../core/src/BandSettingsCache.cpp
        src/PipelineSettings.cpp
        src/RadioSettingsEx.cpp
)

add_library(settings-model-extra STATIC ${BASIC_SETTINGS_SOURCES})

target_include_directories(settings-model-extra PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(settings-model-extra PUBLIC
        settings-model-proto
        settings-model-core
        etl
)
