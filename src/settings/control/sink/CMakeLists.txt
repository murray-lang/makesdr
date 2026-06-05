cmake_minimum_required(VERSION 3.25)

project(makesdr-settings-control-sink)

set(SETTINGS_CONTROL_SINK_SOURCES
        include/settings/control/sink/SettingsControlSink.h
        include/settings/control/sink/PttSink.h
)

add_library(makesdr-settings-control-sink
        INTERFACE
        ${SETTINGS_CONTROL_SINK_SOURCES}
)

target_include_directories(makesdr-settings-control-sink INTERFACE
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

#target_link_libraries(makesdr-settings-control-sink PUBLIC
#        makesdr-settings-control-base
#        makesdr-settings-model-basic
#)