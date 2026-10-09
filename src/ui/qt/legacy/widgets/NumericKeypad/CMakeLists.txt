cmake_minimum_required(VERSION 4.1)

project(ui-qt-widgets-numeric-keypad VERSION 0.1 LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

set (QT_NUMERIC_KEYPAD_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/include/ui/qt/widgets/QtNumericKeypad.h
        ${CMAKE_CURRENT_LIST_DIR}/src/QtNumericKeypad.cpp
)

add_library(ui-qt-widgets-numeric-keypad STATIC ${QT_NUMERIC_KEYPAD_SOURCES})

set_target_properties(ui-qt-widgets-numeric-keypad PROPERTIES AUTOMOC ON)

target_include_directories(ui-qt-widgets-numeric-keypad PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(ui-qt-widgets-numeric-keypad PUBLIC
        ui-qt-widgets-property-setter
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
)
