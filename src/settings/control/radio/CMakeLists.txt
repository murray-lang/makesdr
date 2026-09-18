cmake_minimum_required(VERSION 4.1)

project(settings-control-radio)

set(RADIO_CONTROL_SOURCES
        include/settings/control/radio/RadioControlT.h
)

add_library(settings-control-radio INTERFACE ${RADIO_CONTROL_SOURCES})

target_include_directories(settings-control-radio INTERFACE
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

set(RADIO_CONTROL_LIBS
        config-struct
        settings-control-base
        settings-control-sink
        settings-control-source
        settings-control-factory
)

if (USE_GPIO)
    set(RADIO_CONTROL_LIBS ${RADIO_CONTROL_LIBS} settings-control-hal-digital)
    if (PLATFORM_FAMILY STREQUAL "linux")
        set(RADIO_CONTROL_LIBS ${RADIO_CONTROL_LIBS} core-linux-gpio)
    endif()
endif()



target_link_libraries(settings-control-radio INTERFACE
    ${RADIO_CONTROL_LIBS}
)

## Only for debugging with printf
#target_sources(settings-control-radio PRIVATE
#        ${CMAKE_SOURCE_DIR}/src/core/platform/stm32h745i/app/setup/setup1/cm4/src/syscalls.c
#)
#
## Only for debugging with printf
#target_include_directories(settings-control-radio PRIVATE
#        ${CMAKE_SOURCE_DIR}/src/core/platform/stm32h745i/app/setup/setup1/cm4/include
#)

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()