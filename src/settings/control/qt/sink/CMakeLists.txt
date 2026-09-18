cmake_minimum_required(VERSION 4.1)

project(settings-control-qt-sink)

set(CMAKE_AUTOMOC ON)

set(QT_CONTROL_SOURCE_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/control/qt/QtSettingsControlSinkBase.h
        ${CMAKE_CURRENT_LIST_DIR}/src/QtSettingsControlSinkBase.cpp
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/control/qt/QtSettingsControlSinkT.h
)

add_library(settings-control-qt-sink STATIC ${QT_CONTROL_SOURCE_FILES})

target_include_directories(settings-control-qt-sink PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(settings-control-qt-sink PUBLIC
        config-struct
        settings-control-base
        settings-control-sink
        settings-model-radios-base
)