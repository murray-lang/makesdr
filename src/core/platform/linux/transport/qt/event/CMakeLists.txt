cmake_minimum_required(VERSION 4.1)

project(core-linux-transport-qt-event)

add_subdirectory(base)
add_subdirectory(event-targets)
add_subdirectory(out)
add_subdirectory(in)
add_subdirectory(in-out)
add_subdirectory(radio)

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()
