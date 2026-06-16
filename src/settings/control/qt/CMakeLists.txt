cmake_minimum_required(VERSION 4.1)

project(settings-control-qt)

include(${CMAKE_CURRENT_LIST_DIR}/qt.cmake)

set(CMAKE_AUTOMOC ON)

set(QT_CONTROL_SOURCES
        include/settings/control/qt/QtSettingsControlSink.h
        include/settings/control/qt/QtSettingsControlSource.h
        include/settings/control/qt/QtGlobalControlEventTargets.h
        src/QtSettingsControlSink.cpp
        src/QtSettingsControlSource.cpp
        src/QtGlobalControlEventTargets.cpp
)

add_library(settings-control-qt STATIC ${QT_CONTROL_SOURCES})

target_include_directories(settings-control-qt PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include

)

target_link_libraries(settings-control-qt PUBLIC
        config-struct
        settings-control-base
        settings-control-sink
        settings-control-source
        settings-model-core
        Qt${QT_VERSION_MAJOR}::Core
)