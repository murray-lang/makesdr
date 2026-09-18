cmake_minimum_required(VERSION 4.1)

project(settings-control-transport-source)

set(TRANSPORT_CONTROL_SOURCE_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/control/transport/TransportControlSourceT.h
)

add_library(settings-control-transport-source INTERFACE ${TRANSPORT_CONTROL_SOURCE_FILES})

target_include_directories(settings-control-transport-source INTERFACE
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

if(PLATFORM_FAMILY STREQUAL "linux")
    target_link_libraries(settings-control-transport-source INTERFACE
            settings-model-message
            settings-control-source
            core-linux-transport-qt-event-in
    )
else()
endif()

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()
