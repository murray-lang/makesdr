cmake_minimum_required(VERSION 4.1)

project(RadioSettingsProto-settings-model-core)

set(CORE_SETTINGS_SOURCES
        include/model/core/AutoCompleteTrigger.h
        include/model/core/MessageTagLookup.h
        include/model/core/MessageVisitor.h
        include/model/core/ProtobufIo.h
        include/model/core/SettingFieldPath.h
        include/model/core/SettingFieldUpdate.h
        include/model/core/SettingFieldVariant.h
        include/model/core/SettingsBase.h
        include/model/core/BaseRadioSettings.h

        src/MessageTagLookup.cpp
        src/MessageVisitor.cpp
)

add_library(RadioSettingsProto-settings-model-core STATIC ${CORE_SETTINGS_SOURCES})

target_include_directories(RadioSettingsProto-settings-model-core PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(RadioSettingsProto-settings-model-core PUBLIC
        RadioSettingsProto-settings-model-proto
        etl
)