cmake_minimum_required(VERSION 4.1)

project(core-linux-transport-qt-globals-metering)

find_package(PkgConfig)

find_package(Qt6 REQUIRED COMPONENTS Core)

set(QT_TRANSPORT_GLOBALS_METERING_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/qt/globalQtRxMeteringRings.h
        ${CMAKE_CURRENT_LIST_DIR}/src/globalQtRxMeteringRings.cpp
)

add_library(core-linux-transport-qt-globals-metering STATIC ${QT_TRANSPORT_GLOBALS_METERING_FILES})

target_include_directories(core-linux-transport-qt-globals-metering PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/include
)
target_link_libraries(core-linux-transport-qt-globals-metering PUBLIC
        core-common-transport-metering
        core-linux-transport-qt-base
        core-linux-transport-qt-metering
        Qt6::Core
)