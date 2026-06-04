cmake_minimum_required(VERSION 4.1)

project(makesdr-settings-control-base)

set(SETTINGS_CONTROL_BASE_SOURCES
        include/settings/control/SettingsControlBase.h
)

add_library(makesdr-settings-control-base
        INTERFACE
        ${SETTINGS_CONTROL_BASE_SOURCES}
)

target_include_directories(makesdr-settings-control-base INTERFACE
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

#target_link_libraries(makesdr-settings-control-base PUBLIC
#        makesdr-config
#)