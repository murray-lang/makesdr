cmake_minimum_required(VERSION 4.1)

project(settings-control-qt-source)

set(CMAKE_AUTOMOC ON)

set(QT_CONTROL_SOURCE_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/control/qt/QtSettingsControlSourceT.h
)

add_library(settings-control-qt-source INTERFACE ${QT_CONTROL_SOURCE_FILES})

target_include_directories(settings-control-qt-source INTERFACE
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(settings-control-qt-source INTERFACE
        config-struct
        settings-control-base
        settings-control-source
        settings-model-radios-base
        core-linux-event-data

)