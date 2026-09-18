cmake_minimum_required(VERSION 4.1)

project(core-linux-transport-qt-event-in)

find_package(PkgConfig)

find_package(Qt6 REQUIRED COMPONENTS Core)

set(QT_TRANSPORT_IN_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/in/QtTransportInT.h
)

add_library(core-linux-transport-qt-event-in INTERFACE ${QT_TRANSPORT_IN_FILES})

target_include_directories(core-linux-transport-qt-event-in INTERFACE
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(core-linux-transport-qt-event-in INTERFACE
        config-struct
        core-common-transport
        core-linux-transport-qt-event-base
        core-linux-transport-qt-event-targets
        settings-model-message
        settings-model-radios-iq-dual-split
        settings-model-data-band
        settings-model-data-mode
        settings-model-data-radio
)

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()