cmake_minimum_required(VERSION 4.1)

project(core-linux-transport-qt-event-out)

find_package(PkgConfig)

find_package(Qt6 REQUIRED COMPONENTS Core)

set(QT_TRANSPORT_OUT_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/out/QtTransportOutT.h
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/out/QtTransportFieldUpdateOut.h
)

add_library(core-linux-transport-qt-event-out INTERFACE ${QT_TRANSPORT_OUT_FILES})

target_include_directories(core-linux-transport-qt-event-out INTERFACE
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(core-linux-transport-qt-event-out INTERFACE
        core-common-transport
        core-linux-transport-qt-event-base
        core-linux-transport-qt-event-targets
        settings-model-message
)

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()