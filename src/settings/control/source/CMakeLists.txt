cmake_minimum_required(VERSION 4.1)

project(makesdr-settings-control-source)

set(SETTINGS_CONTROL_SOURCE_SOURCES
        include/settings/control/source/PttSource.h
        include/settings/control/source/SettingsControlSource.h
)

add_library(makesdr-settings-control-source
        INTERFACE
        ${SETTINGS_CONTROL_SOURCE_SOURCES}
)

target_include_directories(makesdr-settings-control-source INTERFACE
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

#target_link_libraries(makesdr-settings-control-source PUBLIC
#        makesdr-settings-control-base
#        makesdr-settings-model-basic
#        makesdr-settings-model-mutable
#)