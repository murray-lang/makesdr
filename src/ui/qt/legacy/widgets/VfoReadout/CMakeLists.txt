cmake_minimum_required(VERSION 4.1)

project(ui-qt-vfo-readout VERSION 0.1 LANGUAGES CXX)

#find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

set (QT_VFO_READOUT_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/include/ui/qt/widgets/QtVfoReadout.h
        ${CMAKE_CURRENT_LIST_DIR}/include/ui/qt/widgets/VfoActions.h
        ${CMAKE_CURRENT_LIST_DIR}/src/QtVfoReadout.cpp
)

add_library(ui-qt-vfo-readout STATIC ${QT_VFO_READOUT_SOURCES})

set_target_properties(ui-qt-vfo-readout PROPERTIES AUTOMOC ON)

target_include_directories(ui-qt-vfo-readout PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(ui-qt-vfo-readout PUBLIC
        ui-qt-widgets-number-readout
        ui-qt-widgets-mini-vfo-toolbar
)
