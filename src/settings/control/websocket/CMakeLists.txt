cmake_minimum_required(VERSION 4.1)

project(settings-control-websocket)

find_package(GnuTLS REQUIRED)

# Configure libwebsockets for minimal embedded build (only for non-STM32 platforms)
set(LWS_WITHOUT_TESTAPPS ON CACHE BOOL "" FORCE)
set(LWS_WITHOUT_TEST_SERVER ON CACHE BOOL "" FORCE)
set(LWS_WITHOUT_TEST_CLIENT ON CACHE BOOL "" FORCE)
set(LWS_WITH_MINIMAL_EXAMPLES OFF CACHE BOOL "" FORCE)
set(LWS_WITH_SSL OFF CACHE BOOL "" FORCE)
set(LWS_WITH_ZLIB OFF CACHE BOOL "" FORCE)
set(LWS_WITH_LIBUV OFF CACHE BOOL "" FORCE)
set(LWS_WITH_LIBEVENT OFF CACHE BOOL "" FORCE)
set(LWS_IPV6 OFF CACHE BOOL "" FORCE)
set(LWS_UNIX_SOCK OFF CACHE BOOL "" FORCE)
set(LWS_WITH_HTTP2 ON CACHE BOOL "" FORCE)
set(LWS_STATIC_PIC ON CACHE BOOL "" FORCE)
add_subdirectory(${WEBSOCKETS_DIR} ${CMAKE_BINARY_DIR}/libwebsockets)

set(SETTINGS_CONTROL_WEBSOCKET_SOURCES
        include/settings/control/websocket/WebSocketControlSource.h
        include/settings/control/websocket/WebSocketControlSink.h
        src/WebSocketControlSource.cpp
        src/WebSocketControlSink.cpp
)

add_library(settings-control-websocket
        ${SETTINGS_CONTROL_WEBSOCKET_SOURCES}
)

target_include_directories(settings-control-websocket PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(settings-control-websocket PUBLIC
        settings-control-base
        settings-control-source
        settings-control-sink
        settings-model-core
        websockets
        GnuTLS::GnuTLS
)
