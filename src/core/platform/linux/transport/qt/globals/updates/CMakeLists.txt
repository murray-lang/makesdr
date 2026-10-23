cmake_minimum_required(VERSION 4.1)

project(core-linux-transport-qt-globals-updates)

find_package(PkgConfig)

find_package(Qt6 REQUIRED COMPONENTS Core)

set(QT_TRANSPORT_GLOBALS_UPDATE_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/qt/globalQtUpdateRings.h
        ${CMAKE_CURRENT_LIST_DIR}/src/globalQtUpdateRings.cpp
)

add_library(core-linux-transport-qt-globals-updates STATIC ${QT_TRANSPORT_GLOBALS_UPDATE_FILES})

target_include_directories(core-linux-transport-qt-globals-updates PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/include
)
target_link_libraries(core-linux-transport-qt-globals-updates PUBLIC
        core-common-transport-updates
        core-linux-transport-qt-base
        core-linux-transport-qt-updates
        Qt6::Core
)