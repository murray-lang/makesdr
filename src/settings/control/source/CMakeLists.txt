cmake_minimum_required(VERSION 4.1)

project(RadioSettingsProto-settings-control-source)

set(SETTINGS_CONTROL_SOURCE_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/include/control/source/PttSource.h
        ${CMAKE_CURRENT_LIST_DIR}/include/control/source/SettingsControlSource.h
)

add_library(RadioSettingsProto-settings-control-source
        STATIC
        ${SETTINGS_CONTROL_SOURCE_SOURCES}
)

target_include_directories(RadioSettingsProto-settings-control-source PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(RadioSettingsProto-settings-control-source PUBLIC
        RadioSettingsProto-settings-control-base
        RadioSettingsProto-settings-model-basic
        RadioSettingsProto-settings-model-mutable
)