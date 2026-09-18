cmake_minimum_required(VERSION 4.1)

project(core-linux-transport-qt-event-radio)

find_package(PkgConfig)

find_package(Qt6 REQUIRED COMPONENTS Core)

set(QT_RADIO_TRANSPORT_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/radio/QtRadioTransportT.h
)

add_library(core-linux-transport-qt-event-radio STATIC ${QT_RADIO_TRANSPORT_FILES})

set_target_properties(core-linux-transport-qt-event-radio PROPERTIES
        AUTOMOC ON
)

target_include_directories(core-linux-transport-qt-event-radio PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(core-linux-transport-qt-event-radio PUBLIC
        Qt6::Core
        settings-model-message
        settings-model-data-band
        settings-model-data-mode
        core-linux-transport-qt-event-in-out
)

#if(BUILD_TESTING)
#    add_subdirectory(tests)
#endif()