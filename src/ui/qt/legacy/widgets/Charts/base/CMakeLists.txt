cmake_minimum_required(VERSION 4.1)

project(ui-qt-widgets-charts-base VERSION 0.1 LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Charts)

set (QT_CHART_BASE_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/include/ui/qt/widgets/QtChartBase.h
        ${CMAKE_CURRENT_LIST_DIR}/include/ui/qt/widgets/QtChartTheme.h
        ${CMAKE_CURRENT_LIST_DIR}/src/QtChartBase.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/QtChartTheme.cpp
)

add_library(ui-qt-widgets-charts-base STATIC ${QT_CHART_BASE_SOURCES})

set_target_properties(ui-qt-widgets-charts-base PROPERTIES AUTOMOC ON)

target_include_directories(ui-qt-widgets-charts-base PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(ui-qt-widgets-charts-base PUBLIC
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        Qt6::Charts
        core-common-samples
)