cmake_minimum_required(VERSION 4.1)

project(ui-qt-widgets-charts-timeseries VERSION 0.1 LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Charts)

set (QT_TIMESERIES_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/include/ui/qt/widgets/QtTimeSeriesChart.h
        ${CMAKE_CURRENT_LIST_DIR}/src/QtTimeSeriesChart.cpp
)

add_library(ui-qt-widgets-charts-timeseries STATIC ${QT_TIMESERIES_SOURCES})

target_include_directories(ui-qt-widgets-charts-timeseries PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(ui-qt-widgets-charts-timeseries PUBLIC
        ui-qt-widgets-charts-base
)