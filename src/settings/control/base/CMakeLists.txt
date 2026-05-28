cmake_minimum_required(VERSION 4.1)

project(RadioSettingsProto-settings-control-base)

set(SETTINGS_CONTROL_BASE_SOURCES
        include/control/SettingsControlBase.h
)

add_library(RadioSettingsProto-settings-control-base
        STATIC
        ${SETTINGS_CONTROL_BASE_SOURCES}
)

target_include_directories(RadioSettingsProto-settings-control-base PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(RadioSettingsProto-settings-control-base PUBLIC
        RadioSettingsProto-config
)