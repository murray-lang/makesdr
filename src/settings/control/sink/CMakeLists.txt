cmake_minimum_required(VERSION 4.1)

project(settings-control-sink)

set(SETTINGS_CONTROL_SINK_SOURCES
        include/settings/control/sink/SettingsControlSinkT.h
        include/settings/control/sink/PttSink.h
)

add_library(settings-control-sink
        INTERFACE
        ${SETTINGS_CONTROL_SINK_SOURCES}
)

target_include_directories(settings-control-sink INTERFACE
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(settings-control-sink INTERFACE
        settings-control-base
        settings-model-radios-base
)