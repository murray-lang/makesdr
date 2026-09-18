cmake_minimum_required(VERSION 4.1)

project(core-linux-transport-qt-event-base)

find_package(PkgConfig)

find_package(Qt6 REQUIRED COMPONENTS Core)

set(TRANSPORT_QT_EVENT_BASE_HEADERS
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/qt/event/MessageEventT.h
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/qt/event/QtTransportBase.h
)

set(TRANSPORT_QT_EVENT_BASE_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/src/QtTransportBase.cpp
)

add_library(core-linux-transport-qt-event-base
        ${TRANSPORT_QT_EVENT_BASE_HEADERS}
        ${TRANSPORT_QT_EVENT_BASE_SOURCES}
)

target_include_directories(core-linux-transport-qt-event-base PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(core-linux-transport-qt-event-base PUBLIC
        settings-model-message
        Qt6::Core
)

set_target_properties(core-linux-transport-qt-event-base PROPERTIES
        AUTOMOC ON
)