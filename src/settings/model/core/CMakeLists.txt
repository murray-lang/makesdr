cmake_minimum_required(VERSION 3.25)

project(makesdr-settings-model-core)

set(CORE_SETTINGS_SOURCES
        include/settings/model/core/AutoCompleteTrigger.h
        include/settings/model/core/MessageTagLookup.h
        include/settings/model/core/MessageVisitor.h
        include/settings/model/core/ProtobufIo.h
        include/settings/model/core/RadioSettings.h
        include/settings/model/core/RadioSettingsSink.h
        include/settings/model/core/RadioSettingsSource.h
        include/settings/model/core/SettingFieldPath.h
        include/settings/model/core/SettingFieldUpdate.h
        include/settings/model/core/SettingFieldUpdateSink.h
        include/settings/model/core/SettingFieldUpdateSource.h
        include/settings/model/core/SettingFieldVariant.h
        include/settings/model/core/SettingsBase.h
        include/settings/model/core/SplitBandId.h
        include/settings/model/core/PipelineId.h

        src/RadioSettings.cpp
        src/MessageTagLookup.cpp
        src/MessageVisitor.cpp
)

add_library(makesdr-settings-model-core STATIC ${CORE_SETTINGS_SOURCES})

target_include_directories(makesdr-settings-model-core PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(makesdr-settings-model-core PUBLIC
        makesdr-settings-model-proto
        makesdr-settings-model-meta
        etl
)