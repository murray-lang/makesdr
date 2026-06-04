cmake_minimum_required(VERSION 4.1)

project(makesdr-settings-control-radio)

set(RADIO_CONTROL_SOURCES
        include/settings/control/radio/RadioControl.h
        ${CMAKE_CURRENT_LIST_DIR}/src/RadioControl.cpp
)

add_library(makesdr-settings-control-radio STATIC ${RADIO_CONTROL_SOURCES})

target_include_directories(makesdr-settings-control-radio PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(makesdr-settings-control-radio PUBLIC
        makesdr-config-struct
        makesdr-settings-control-hal-usb
        makesdr-settings-control-base
        makesdr-settings-control-sink
        makesdr-settings-control-source
        makesdr-settings-control-factory
        makesdr-settings-model-core
)