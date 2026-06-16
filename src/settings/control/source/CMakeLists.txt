cmake_minimum_required(VERSION 4.1)

project(settings-control-source)

set(SETTINGS_CONTROL_SOURCE_SOURCES
        include/settings/control/source/PttSource.h
        include/settings/control/source/SettingsControlSource.h
)

add_library(settings-control-source
        INTERFACE
        ${SETTINGS_CONTROL_SOURCE_SOURCES}
)

target_include_directories(settings-control-source INTERFACE
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

#target_link_libraries(settings-control-source PUBLIC
#        settings-control-base
#        settings-model-basic
#        settings-model-mutable
#)