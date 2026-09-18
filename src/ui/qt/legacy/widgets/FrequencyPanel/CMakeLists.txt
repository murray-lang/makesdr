cmake_minimum_required(VERSION 4.1)

project(ui-qt-widgets-frequency-panel VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

set (QT_UTIL_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/include/ui/qt/widgets/QtBandReadout.h
        ${CMAKE_CURRENT_LIST_DIR}/include/ui/qt/widgets/QtFrequencyPanel.h

        ${CMAKE_CURRENT_LIST_DIR}/src/QtBandReadout.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/QtFrequencyPanel.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/QtFrequencyPanel.ui
)

add_library(ui-qt-widgets-frequency-panel STATIC ${QT_UTIL_SOURCES})

target_include_directories(ui-qt-widgets-frequency-panel PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(ui-qt-widgets-frequency-panel PUBLIC
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        settings-model-radios-iq-dual-split
        ui-qt-vfo-readout
        ui-qt-widgets-property-setter
)
