cmake_minimum_required(VERSION 4.1)

project(core-linux-transport-qt-globals-spectrum)

find_package(PkgConfig)

find_package(Qt6 REQUIRED COMPONENTS Core)

set(QT_TRANSPORT_GLOBALS_SPECTRUM_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/qt/globalQtSpectrumRings.h
        ${CMAKE_CURRENT_LIST_DIR}/src/globalQtSpectrumRings.cpp
)

add_library(core-linux-transport-qt-globals-spectrum STATIC ${QT_TRANSPORT_GLOBALS_SPECTRUM_FILES})

target_include_directories(core-linux-transport-qt-globals-spectrum PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/include
)
target_link_libraries(core-linux-transport-qt-globals-spectrum PUBLIC
        core-common-transport-spectrum
        core-linux-transport-qt-base
        core-linux-transport-qt-spectrum
        Qt6::Core
)