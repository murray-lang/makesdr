cmake_minimum_required(VERSION 4.1)

project(ui-qt-widgets-number-readout VERSION 0.1 LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

set (QT_NUMBER_READOUT_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/include/ui/qt/widgets/QtNumberReadout.h
        ${CMAKE_CURRENT_LIST_DIR}/src/QtNumberReadout.cpp
)

add_library(ui-qt-widgets-number-readout STATIC ${QT_NUMBER_READOUT_SOURCES})

set_target_properties(ui-qt-widgets-number-readout PROPERTIES AUTOMOC ON)

target_include_directories(ui-qt-widgets-number-readout PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(ui-qt-widgets-number-readout PUBLIC
        core-common-samples
        ui-qt-widgets-property-setter
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
)
