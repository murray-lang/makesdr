cmake_minimum_required(VERSION 4.1)

project(settings-control-transport-sink)

set(TRANSPORT_CONTROL_SINK_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/control/transport/TransportControlSinkT.h
)

add_library(settings-control-transport-sink INTERFACE ${TRANSPORT_CONTROL_SINK_FILES})

target_include_directories(settings-control-transport-sink INTERFACE
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

if(PLATFORM_FAMILY STREQUAL "linux")
    target_link_libraries(settings-control-transport-sink INTERFACE
            settings-control-sink
            core-linux-transport-qt-event-out
    )
else()
endif()

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()
