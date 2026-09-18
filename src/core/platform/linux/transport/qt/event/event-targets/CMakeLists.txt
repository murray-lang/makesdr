cmake_minimum_required(VERSION 4.1)

project(core-linux-transport-qt-event-targets)

find_package(PkgConfig)

find_package(Qt6 REQUIRED COMPONENTS Core)

set(TRANSPORT_QT_EVENT_TARGET_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/qt/event/globalQtEventTargets.h
        ${CMAKE_CURRENT_LIST_DIR}/src/globalQtEventTargets.cpp
)

add_library(core-linux-transport-qt-event-targets STATIC ${TRANSPORT_QT_EVENT_TARGET_FILES})

target_include_directories(core-linux-transport-qt-event-targets PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(core-linux-transport-qt-event-targets PUBLIC
        config-struct
        Qt6::Core
)