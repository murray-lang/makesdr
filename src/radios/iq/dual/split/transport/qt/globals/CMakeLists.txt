cmake_minimum_required(VERSION 4.1)

project(radios-iq-dual-split-transport-qt-globals)

find_package(PkgConfig)

find_package(Qt6 REQUIRED COMPONENTS Core)

set(QT_TRANSPORT_GLOBALS_IQ_DUAL_SPLIT_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/qt/globalQtDualIqSplitRings.h
        ${CMAKE_CURRENT_LIST_DIR}/src/globalQtDualIqSplitRings.cpp
)

add_library(radios-iq-dual-split-transport-qt-globals STATIC ${QT_TRANSPORT_GLOBALS_IQ_DUAL_SPLIT_FILES})

target_include_directories(radios-iq-dual-split-transport-qt-globals PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/include
)
target_link_libraries(radios-iq-dual-split-transport-qt-globals PUBLIC
        core-common-transport-iq
        core-linux-transport-qt-base
        radios-iq-dual-split-transport-qt-radio
        radios-iq-dual-split-settings-model
        Qt6::Core
)
