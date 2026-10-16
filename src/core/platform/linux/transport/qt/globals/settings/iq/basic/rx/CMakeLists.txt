cmake_minimum_required(VERSION 4.1)

project(core-linux-transport-qt-globals-iq-basic-rx)

find_package(PkgConfig)

find_package(Qt6 REQUIRED COMPONENTS Core)

set(QT_TRANSPORT_GLOBALS_IQ_BASIC_RX_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/qt/globalQtBasicIqRxRings.h
        ${CMAKE_CURRENT_LIST_DIR}/src/globalQtBasicIqRxRings.cpp
)

add_library(core-linux-transport-qt-globals-iq-basic-rx STATIC ${QT_TRANSPORT_GLOBALS_IQ_BASIC_RX_FILES})

target_include_directories(core-linux-transport-qt-globals-iq-basic-rx PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/include
)
target_link_libraries(core-linux-transport-qt-globals-iq-basic-rx PUBLIC
        core-common-transport-iq
        core-linux-transport-qt-base
        settings-model-radio-iq-basic-rx
        Qt6::Core
)