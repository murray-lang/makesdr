cmake_minimum_required(VERSION 4.1)

project(ui-qt-widgets-band-dialog VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

set (QT_UTIL_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/include/ui/qt/widgets/QtBandDialog.h
        ${CMAKE_CURRENT_LIST_DIR}/src/QtBandDialog.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/QtBandDialog.ui
)

add_library(ui-qt-widgets-band-dialog STATIC ${QT_UTIL_SOURCES})

target_include_directories(ui-qt-widgets-band-dialog PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(ui-qt-widgets-band-dialog PUBLIC
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        config-json
        settings-model-radios-base
        settings-model-data-band
)