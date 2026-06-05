cmake_minimum_required(VERSION 3.25)

project(makesdr-settings-control-hal-funcube)

set(FUNCUBE_SOURCES
        include/settings/control/FunCubeDongle/FCDHidCmd.h
        include/settings/control/FunCubeDongle/FunCubeDongle.h
        ${CMAKE_CURRENT_LIST_DIR}/FunCubeDongle.cpp
)

add_library(makesdr-settings-control-hal-funcube STATIC ${FUNCUBE_SOURCES})

target_include_directories(makesdr-settings-control-hal-funcube PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(makesdr-settings-control-hal-funcube PUBLIC
        makesdr-settings-control-hal-usb
        makesdr-config-struct
        makesdr-settings-control-base
        makesdr-settings-control-sink
        makesdr-settings-model-core
#        makesdr-settings-model-basic
)