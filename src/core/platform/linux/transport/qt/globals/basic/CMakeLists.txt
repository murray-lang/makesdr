cmake_minimum_required(VERSION 4.1)

project(core-linux-transport-qt-globals-basic)

find_package(PkgConfig)

find_package(Qt6 REQUIRED COMPONENTS Core)

set(QT_TRANSPORT_GLOBALS_BASIC_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/qt/globalQtBasicRings.h
        ${CMAKE_CURRENT_LIST_DIR}/src/globalQtBasicRings.cpp
)

add_library(core-linux-transport-qt-globals-basic STATIC ${QT_TRANSPORT_GLOBALS_BASIC_FILES})

target_include_directories(core-linux-transport-qt-globals-basic PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/include
)
target_link_libraries(core-linux-transport-qt-globals-basic PUBLIC
        core-common-transport-basic
        core-linux-transport-qt-base
        core-linux-transport-qt-basic
        Qt6::Core
)