cmake_minimum_required(VERSION 4.1)

project(example-play)

set(PLAY_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/main.cpp
        ${CMAKE_CURRENT_LIST_DIR}/../../data/exampleRadioSettings.h
        ${CMAKE_CURRENT_LIST_DIR}/../../data/exampleRadioSettings.cpp
)

add_executable(example-play
     ${PLAY_SOURCES}
)

target_include_directories(example-play PUBLIC
        ${INCLUDE_DIR}
)

target_link_libraries(example-play PUBLIC
        settings-model-proto
        settings-model-core
        settings-model-extra
        settings-model-meta
)