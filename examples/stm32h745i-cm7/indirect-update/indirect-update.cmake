cmake_minimum_required(VERSION 4.1)

project(example-cm7-indirect-update)


if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(DEBUG)
endif()

set(PLAY_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/main.cpp
        ${CMAKE_CURRENT_LIST_DIR}/../../data/exampleRadioSettings.h
        ${CMAKE_CURRENT_LIST_DIR}/../../data/exampleRadioSettings.cpp
)

add_executable(example-cm7-indirect-update
     ${PLAY_SOURCES}
)

target_include_directories(example-cm7-indirect-update PUBLIC
        ${INCLUDE_DIR}
)

target_link_libraries(example-cm7-indirect-update PUBLIC
        settings-model-core
        core-stm32h745i-app-setup-setup1-cm7
)

add_custom_command(TARGET example-cm7-indirect-update POST_BUILD
        COMMAND ${CMAKE_SIZE} $<TARGET_FILE:example-cm7-indirect-update>
)