cmake_minimum_required(VERSION 4.1)

project(example-linux-payload-update)

set(PLAY_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/main.cpp
        ${CMAKE_CURRENT_LIST_DIR}/../../data/exampleRadioSettings.h
        ${CMAKE_CURRENT_LIST_DIR}/../../data/exampleRadioSettings.cpp
)

add_executable(example-linux-payload-update
     ${PLAY_SOURCES}
)

target_include_directories(example-linux-payload-update PUBLIC
        ${INCLUDE_DIR}
)

target_link_libraries(example-linux-payload-update PUBLIC
        settings-model-proto
        settings-model-core
        settings-model-meta
        settings-control-radio
)