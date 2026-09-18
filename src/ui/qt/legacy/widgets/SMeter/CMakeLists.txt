cmake_minimum_required(VERSION 4.1)

project(ui-qt-widgets-s-meter VERSION 0.1 LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

set (QT_PROPERTY_SETTER_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/include/ui/qt/widgets/QtSMeter.h
        ${CMAKE_CURRENT_LIST_DIR}/include/ui/qt/widgets/SMeterTickProvider.h

        ${CMAKE_CURRENT_LIST_DIR}/src/QtSMeter.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/SMeterTickProvider.cpp
)

add_library(ui-qt-widgets-s-meter STATIC ${QT_PROPERTY_SETTER_SOURCES})

set_target_properties(ui-qt-widgets-s-meter PROPERTIES AUTOMOC ON)

target_include_directories(ui-qt-widgets-s-meter PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_include_directories(ui-qt-widgets-s-meter PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/include/ui/qt/widgets
)

target_link_libraries(ui-qt-widgets-s-meter PUBLIC
        ui-qt-widgets-scale-renderer
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
)