cmake_minimum_required(VERSION 4.1)

project(core-linux-event-targets)

find_package(PkgConfig)

find_package(Qt6 REQUIRED COMPONENTS Core)

set(QT_EVENT_TARGET_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/event/targets/QtDataEventTargets.h
        ${CMAKE_CURRENT_LIST_DIR}/src/QtDataEventTargets.cpp

)

add_library(core-linux-event-targets OBJECT ${QT_EVENT_TARGET_FILES})

target_include_directories(core-linux-event-targets PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(core-linux-event-targets PUBLIC
        core-common-event
        Qt6::Core
)