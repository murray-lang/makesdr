cmake_minimum_required(VERSION 4.1)

project(settings-control)

add_subdirectory(base)
add_subdirectory(sink)
add_subdirectory(source)
add_subdirectory(hal)
add_subdirectory(radio)
if (PLATFORM_FAMILY STREQUAL "linux")
    add_subdirectory(qt)
    add_subdirectory(websocket)
endif()
add_subdirectory(factory)
