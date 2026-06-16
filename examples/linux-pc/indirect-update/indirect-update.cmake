cmake_minimum_required(VERSION 4.1)

project(example-linux-indirect-update)

set(PLAY_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/main.cpp
        ${CMAKE_CURRENT_LIST_DIR}/../../data/exampleRadioSettings.h
        ${CMAKE_CURRENT_LIST_DIR}/../../data/exampleRadioSettings.cpp
)

add_executable(example-linux-indirect-update
     ${PLAY_SOURCES}
)

target_include_directories(example-linux-indirect-update PUBLIC
        ${INCLUDE_DIR}
)

target_link_libraries(example-linux-indirect-update PUBLIC
        settings-model-proto
        settings-model-core
        settings-model-meta
)