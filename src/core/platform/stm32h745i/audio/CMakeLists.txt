cmake_minimum_required(VERSION 4.1)

project(core-stm32h745i-audio)

set (STM32_AUDIO_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/audio/AudioOutputVariant.h
        ${CMAKE_CURRENT_LIST_DIR}/include/audio/AudioOutputFactory.h
        ${CMAKE_CURRENT_LIST_DIR}/src/AudioOutputFactory.cpp
)

add_library(core-stm32h745i-audio STATIC ${STM32_AUDIO_FILES})

target_include_directories(core-stm32h745i-audio PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(core-stm32h745i-audio PUBLIC
        config-struct
        etl::etl
)