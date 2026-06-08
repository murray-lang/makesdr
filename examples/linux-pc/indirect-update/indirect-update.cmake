cmake_minimum_required(VERSION 3.25)

project(makesdr-example-indirect-update)

set(PLAY_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/main.cpp
        ${CMAKE_CURRENT_LIST_DIR}/../../data/exampleRadioSettings.h
        ${CMAKE_CURRENT_LIST_DIR}/../../data/exampleRadioSettings.cpp
)

add_executable(makesdr-example-indirect-update
     ${PLAY_SOURCES}
)

target_include_directories(makesdr-example-indirect-update PUBLIC
        ${INCLUDE_DIR}
)

target_link_libraries(makesdr-example-indirect-update PUBLIC
        makesdr-settings-model-proto
        makesdr-settings-model-core
        makesdr-settings-model-meta
)