cmake_minimum_required(VERSION 4.1)

project(settings-control-radio)

set(RADIO_CONTROL_SOURCES
        include/settings/control/radio/RadioControl.h
        ${CMAKE_CURRENT_LIST_DIR}/src/RadioControl.cpp
)

add_library(settings-control-radio STATIC ${RADIO_CONTROL_SOURCES})

target_include_directories(settings-control-radio PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

set(RADIO_CONTROL_LIBS
        config-struct
        settings-control-hal-usb
        settings-control-hal-gpio
        settings-control-base
        settings-control-sink
        settings-control-source
        settings-control-factory
        settings-model-core
)

if (PLATFORM_FAMILY STREQUAL "linux")
    set(RADIO_CONTROL_LIBS ${RADIO_CONTROL_LIBS} core-linux-gpio)
endif()

target_link_libraries(settings-control-radio PUBLIC
    ${RADIO_CONTROL_LIBS}
)