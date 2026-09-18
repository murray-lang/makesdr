cmake_minimum_required(VERSION 4.1)

project(ui-qt-widgets-stepper VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

set (QT_STEPPER_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/include/ui/qt/widgets/QtStepper.h
        ${CMAKE_CURRENT_LIST_DIR}/src/QtStepper.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/QtStepper.ui
)

add_library(ui-qt-widgets-stepper STATIC ${QT_STEPPER_SOURCES})

target_include_directories(ui-qt-widgets-stepper PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}
)

target_include_directories(ui-qt-widgets-stepper PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/include/ui/qt/widgets
)

target_link_libraries(ui-qt-widgets-stepper PUBLIC
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        ui-qt-widgets-property-setter
)
