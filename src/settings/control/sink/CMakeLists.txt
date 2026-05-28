cmake_minimum_required(VERSION 4.1)

project(RadioSettingsProto-settings-control-sink)

set(SETTINGS_CONTROL_SINK_SOURCES
        include/control/sink/SettingsControlSink.h
        include/control/sink/PttSink.h
)

add_library(RadioSettingsProto-settings-control-sink
        STATIC
        ${SETTINGS_CONTROL_SINK_SOURCES}
)

target_include_directories(RadioSettingsProto-settings-control-sink PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(RadioSettingsProto-settings-control-sink PUBLIC
        RadioSettingsProto-settings-control-base
        RadioSettingsProto-settings-model-basic
)