cmake_minimum_required(VERSION 4.1)

project(core-linux-transport-qt-globals-meta-rx)

find_package(PkgConfig)

find_package(Qt6 REQUIRED COMPONENTS Core)

set(QT_TRANSPORT_GLOBALS_BASIC_RX_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/qt/globalQtBasicRxRings.h
        ${CMAKE_CURRENT_LIST_DIR}/src/globalQtBasicRxRings.cpp
)

add_library(core-linux-transport-qt-globals-meta-rx STATIC ${QT_TRANSPORT_GLOBALS_BASIC_RX_FILES})

target_include_directories(core-linux-transport-qt-globals-meta-rx PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/include
)
target_link_libraries(core-linux-transport-qt-globals-meta-rx PUBLIC
        core-common-transport-meta
        core-linux-transport-qt-base
        settings-model-radio-basic-rx
        Qt6::Core
)