cmake_minimum_required(VERSION 4.1)

project(radios-iq-dual-rx-transport-qt-globals)

find_package(PkgConfig)

find_package(Qt6 REQUIRED COMPONENTS Core)

set(QT_TRANSPORT_GLOBALS_IQ_DUAL_RX_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/qt/globalQtDualIqRxRings.h
        ${CMAKE_CURRENT_LIST_DIR}/src/globalQtDualIqRxRings.cpp
)

add_library(radios-iq-dual-rx-transport-qt-globals STATIC ${QT_TRANSPORT_GLOBALS_IQ_DUAL_RX_FILES})

target_include_directories(radios-iq-dual-rx-transport-qt-globals PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/include
)
target_link_libraries(radios-iq-dual-rx-transport-qt-globals PUBLIC
        core-common-transport-iq
        core-linux-transport-qt-base
        radios-iq-dual-rx-transport-qt-radio
        radios-iq-dual-rx-settings-model
        Qt6::Core
)