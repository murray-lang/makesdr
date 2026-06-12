cmake_minimum_required(VERSION 3.25)

project(example-cm7-usb-mmc)


if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(DEBUG)
endif()

set(PLAY_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/main.cpp
)

add_executable(example-cm7-usb-mmc
        ${PLAY_SOURCES}
)

target_include_directories(example-cm7-usb-mmc PUBLIC
        ${INCLUDE_DIR}
)

target_link_libraries(example-cm7-usb-mmc PUBLIC
        core-sys-stm32h745i-app-cm7
)

add_custom_command(TARGET example-cm7-usb-mmc POST_BUILD
        COMMAND ${CMAKE_SIZE} $<TARGET_FILE:example-cm7-usb-mmc>
)

set(EXTERNAL_LOADER "${CMAKE_CURRENT_LIST_DIR}/MT25TL01G_STM32H745I-DISCO.stldr")


add_custom_target(FLASH_CM7
        COMMAND STM32_Programmer_CLI --connect port=SWD -el "${EXTERNAL_LOADER}" --erase all --write ${CMAKE_BINARY_DIR}/example-cm7-usb-mmc.elf --verify --start
        DEPENDS ${example-cm7-usb-mmc}
)