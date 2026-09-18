cmake_minimum_required(VERSION 4.1)

project(ui-qt-main-window)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

set(QT_MAIN_WINDOW_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/ui/qt/MainWindow.h
        ${CMAKE_CURRENT_LIST_DIR}/src/MainWindow.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/mainwindow-1024x600.ui
)

add_library(ui-qt-main-window STATIC ${QT_MAIN_WINDOW_FILES})

target_include_directories(ui-qt-main-window PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_include_directories(ui-qt-main-window PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/include/ui/qt
)

target_link_libraries(ui-qt-main-window PUBLIC
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        settings-model-radios-iq-dual-split
        ui-qt-widgets-band-dialog
        ui-qt-widgets-stepper
        ui-qt-faces-factory
        ui-qt-faces-standard
        ui-qt-radio-client
        ui-qt-icons
)