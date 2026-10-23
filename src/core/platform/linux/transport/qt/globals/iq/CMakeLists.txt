cmake_minimum_required(VERSION 4.1)

project(core-linux-transport-qt-globals-iq)

find_package(PkgConfig)

find_package(Qt6 REQUIRED COMPONENTS Core)

set(QT_TRANSPORT_GLOBALS_IQ_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/qt/globalQtIqRings.h
        ${CMAKE_CURRENT_LIST_DIR}/src/globalQtIqRings.cpp
)

add_library(core-linux-transport-qt-globals-iq STATIC ${QT_TRANSPORT_GLOBALS_IQ_FILES})

target_include_directories(core-linux-transport-qt-globals-iq PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/include
)
target_link_libraries(core-linux-transport-qt-globals-iq PUBLIC
        core-common-transport-iq
        core-linux-transport-qt-base
        core-linux-transport-qt-iq
        Qt6::Core
)