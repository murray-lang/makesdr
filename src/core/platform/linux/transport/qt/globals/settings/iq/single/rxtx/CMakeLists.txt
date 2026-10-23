cmake_minimum_required(VERSION 4.1)

project(core-linux-transport-qt-globals-iq-basic-rxtx)

find_package(PkgConfig)

find_package(Qt6 REQUIRED COMPONENTS Core)

set(QT_TRANSPORT_GLOBALS_IQ_BASIC_RXTX_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/qt/globalQtSingleIqRxTxRings.h
        ${CMAKE_CURRENT_LIST_DIR}/src/globalQtSingleIqRxTxRings.cpp
)

add_library(core-linux-transport-qt-globals-iq-basic-rxtx STATIC ${QT_TRANSPORT_GLOBALS_IQ_BASIC_RXTX_FILES})

target_include_directories(core-linux-transport-qt-globals-iq-basic-rxtx PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/include
)
target_link_libraries(core-linux-transport-qt-globals-iq-basic-rxtx PUBLIC
        core-common-transport-iq
        core-linux-transport-qt-base
        settings-model-radio-iq-single-rxtx
        Qt6::Core
)