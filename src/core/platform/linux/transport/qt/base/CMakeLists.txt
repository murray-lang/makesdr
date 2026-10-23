cmake_minimum_required(VERSION 4.1)

project(core-linux-transport-qt-base)

find_package(PkgConfig)

find_package(Qt6 REQUIRED COMPONENTS Core)

set(TRANSPORT_QT_BASE_HEADERS
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/qt/MessageEvent.h
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/qt/QtNotifier.h
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/qt/QtTransportBase.h
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/qt/QtRecipientT.h
)

set(TRANSPORT_QT_BASE_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/src/QtTransportBase.cpp
)

add_library(core-linux-transport-qt-base
        ${TRANSPORT_QT_BASE_HEADERS}
        ${TRANSPORT_QT_BASE_SOURCES}
)

target_include_directories(core-linux-transport-qt-base PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(core-linux-transport-qt-base PUBLIC
        settings-model-message
        core-common-transport-base
        Qt6::Core
)

set_target_properties(core-linux-transport-qt-base PROPERTIES
        AUTOMOC ON
)