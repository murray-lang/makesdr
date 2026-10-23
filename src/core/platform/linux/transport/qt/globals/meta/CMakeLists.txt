cmake_minimum_required(VERSION 4.1)

project(core-linux-transport-qt-globals-meta)

find_package(PkgConfig)

find_package(Qt6 REQUIRED COMPONENTS Core)

set(QT_TRANSPORT_GLOBALS_META_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/qt/globalQtMetaRings.h
        ${CMAKE_CURRENT_LIST_DIR}/src/globalQtMetaRings.cpp
)

add_library(core-linux-transport-qt-globals-meta STATIC ${QT_TRANSPORT_GLOBALS_META_FILES})

target_include_directories(core-linux-transport-qt-globals-meta PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/include
)
target_link_libraries(core-linux-transport-qt-globals-meta PUBLIC
        core-common-transport-meta
        core-linux-transport-qt-base
        core-linux-transport-qt-meta
        Qt6::Core
)