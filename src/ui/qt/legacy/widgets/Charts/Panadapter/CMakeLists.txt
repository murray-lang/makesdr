cmake_minimum_required(VERSION 4.1)

project(ui-qt-widgets-charts-panadapter VERSION 0.1 LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Charts)

set (QT_PANADAPTER_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/include/ui/qt/widgets/QtPanadapter.h
        ${CMAKE_CURRENT_LIST_DIR}/src/QtPanadapter.cpp
)

add_library(ui-qt-widgets-charts-panadapter STATIC ${QT_PANADAPTER_SOURCES})

target_include_directories(ui-qt-widgets-charts-panadapter PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(ui-qt-widgets-charts-panadapter PUBLIC
        ui-qt-widgets-charts-base
        core-common-dsp-transforms-fft-liquid
)