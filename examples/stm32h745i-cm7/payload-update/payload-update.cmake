cmake_minimum_required(VERSION 4.1)

project(example-cm7-payload-update)

set(PLAY_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/main.cpp
        ${CMAKE_CURRENT_LIST_DIR}/../../data/exampleRadioSettings.h
        ${CMAKE_CURRENT_LIST_DIR}/../../data/exampleRadioSettings.cpp
)

set(LD_FILE_CM7 ${CMAKE_CURRENT_LIST_DIR}/STM32H745XIHX_QSPI.ld)

add_executable(example-cm7-payload-update
     ${PLAY_SOURCES}
)

target_include_directories(example-cm7-payload-update PUBLIC
        ${INCLUDE_DIR}
)

target_link_libraries(example-cm7-payload-update PUBLIC
        -T${LD_FILE_CM7}
        settings-model-proto
        settings-model-core
        settings-model-meta
        core-stm32h745i-app-setup-setup1-cm7
)

add_custom_command(TARGET example-cm7-payload-update POST_BUILD
        COMMAND ${CMAKE_SIZE} $<TARGET_FILE:example-cm7-payload-update>
)

set(EXTERNAL_LOADER "${CMAKE_CURRENT_LIST_DIR}/MT25TL01G_STM32H745I-DISCO.stldr")


add_custom_target(FLASH_example-cm7-payload-update
        COMMAND STM32_Programmer_CLI --connect port=SWD -el "${EXTERNAL_LOADER}" --erase all --write ${CMAKE_BINARY_DIR}/example-cm7-payload-update.elf --verify --start
        DEPENDS example-cm7-payload-update
)