cmake_minimum_required(VERSION 4.1)

project(settings-control-hal-usb-funcube)

set(FUNCUBE_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/control/FunCubeDongle/FCDHidCmd.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/control/FunCubeDongle/FunCubeDongleT.h
)

set(FUNCUBE_LIBS
        config-struct
        settings-control-base
        settings-control-sink
        settings-model-radio-base
)

if (PLATFORM_FAMILY STREQUAL "linux")
    set(FUNCUBE_LIBS ${FUNCUBE_LIBS} core-linux-usb-host)
elseif(TARGET_PLATFORM STREQUAL "stm32h745i-cm7")
    set(FUNCUBE_LIBS ${FUNCUBE_LIBS} core-freertos-usb-host)
elseif(TARGET_PLATFORM STREQUAL "stm32h745i-cm4")
    set(FUNCUBE_LIBS ${FUNCUBE_LIBS} core-freertos-usb-host)
endif()

add_library(settings-control-hal-usb-funcube INTERFACE ${FUNCUBE_SOURCES})

target_include_directories(settings-control-hal-usb-funcube INTERFACE
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(settings-control-hal-usb-funcube INTERFACE ${FUNCUBE_LIBS})