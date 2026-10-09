cmake_minimum_required(VERSION 4.1)

project(ui-qt-widgets-charts-waterfall VERSION 0.1 LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

set (QT_WATERFALL_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/include/ui/qt/widgets/QtWaterfall.h
        ${CMAKE_CURRENT_LIST_DIR}/src/QtWaterfall.cpp
)

add_library(ui-qt-widgets-charts-waterfall STATIC ${QT_WATERFALL_SOURCES})

set_target_properties(ui-qt-widgets-charts-waterfall PROPERTIES AUTOMOC ON)

target_include_directories(ui-qt-widgets-charts-waterfall PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(ui-qt-widgets-charts-waterfall PUBLIC
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        core-common-samples
        ui-common-waterfall
)
