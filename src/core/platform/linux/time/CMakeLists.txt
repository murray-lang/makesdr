cmake_minimum_required(VERSION 4.1)

project(core-linux-time)

set(LINUX_TIME_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/src/Timestamp.cpp
)

add_library(core-linux-time INTERFACE ${LINUX_TIME_SOURCES})

target_link_libraries(core-linux-time INTERFACE core-common-time)