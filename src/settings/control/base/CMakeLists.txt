cmake_minimum_required(VERSION 3.25)

project(settings-control-base)

set(SETTINGS_CONTROL_BASE_SOURCES
        include/settings/control/SettingsControlBase.h
)

add_library(settings-control-base
        INTERFACE
        ${SETTINGS_CONTROL_BASE_SOURCES}
)

target_include_directories(settings-control-base INTERFACE
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

#target_link_libraries(settings-control-base PUBLIC
#        config
#)