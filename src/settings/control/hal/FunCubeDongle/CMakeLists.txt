cmake_minimum_required(VERSION 4.1)

project(RadioSettingsProto-settings-control-hal-funcube)

set(FUNCUBE_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/FCDHidCmd.h
        ${CMAKE_CURRENT_LIST_DIR}/FunCubeDongle.h
        ${CMAKE_CURRENT_LIST_DIR}/FunCubeDongle.cpp
)

add_library(RadioSettingsProto-settings-control-hal-funcube STATIC ${FUNCUBE_SOURCES})

target_link_libraries(RadioSettingsProto-settings-control-hal-funcube PUBLIC
        RadioSettingsProto-settings-control-hal-usb
        RadioSettingsProto-config
        RadioSettingsProto-settings-control-base
        RadioSettingsProto-settings-control-sink
        RadioSettingsProto-settings-model-core
        RadioSettingsProto-settings-model-basic
)