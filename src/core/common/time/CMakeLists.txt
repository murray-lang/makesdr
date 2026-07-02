cmake_minimum_required(VERSION 4.1)

project(core-common-time)

set(COMMON_TIME_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/include/time/Timestamp.h
)

add_library(core-common-time INTERFACE ${COMMON_TIME_SOURCES})

target_include_directories(core-common-time INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)