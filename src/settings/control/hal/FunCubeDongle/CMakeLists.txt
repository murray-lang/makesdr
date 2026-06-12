cmake_minimum_required(VERSION 3.25)

project(settings-control-hal-funcube)

set(FUNCUBE_SOURCES
        include/settings/control/FunCubeDongle/FCDHidCmd.h
        include/settings/control/FunCubeDongle/FunCubeDongle.h
        ${CMAKE_CURRENT_LIST_DIR}/FunCubeDongle.cpp
)

add_library(settings-control-hal-funcube STATIC ${FUNCUBE_SOURCES})

target_include_directories(settings-control-hal-funcube PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(settings-control-hal-funcube PUBLIC
        settings-control-hal-usb
        config-struct
        settings-control-base
        settings-control-sink
        settings-model-core
#        settings-model-basic
)