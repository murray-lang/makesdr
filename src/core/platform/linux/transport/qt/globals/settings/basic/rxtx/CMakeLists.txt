cmake_minimum_required(VERSION 4.1)

project(core-linux-transport-qt-globals-basic-rxtx)

find_package(PkgConfig)

find_package(Qt6 REQUIRED COMPONENTS Core)

set(QT_TRANSPORT_GLOBALS_BASIC_RXTX_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/qt/globalQtBasicRxTxRings.h
        ${CMAKE_CURRENT_LIST_DIR}/src/globalQtBasicRxTxRings.cpp
)

add_library(core-linux-transport-qt-globals-basic-rxtx STATIC ${QT_TRANSPORT_GLOBALS_BASIC_RXTX_FILES})

target_include_directories(core-linux-transport-qt-globals-basic-rxtx PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/include
)
target_link_libraries(core-linux-transport-qt-globals-basic-rxtx PUBLIC
        core-common-transport-basic
        core-linux-transport-qt-base
        settings-model-radio-basic-rxtx
        Qt6::Core
)