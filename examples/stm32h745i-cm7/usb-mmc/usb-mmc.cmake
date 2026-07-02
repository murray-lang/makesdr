cmake_minimum_required(VERSION 4.1)

project(example-cm7-usb-mmc)


if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(DEBUG)
endif()

set(PLAY_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/main.cpp
)

set(LD_FILE_CM7 ${CMAKE_CURRENT_LIST_DIR}/STM32H745XIHX_QSPI.ld)

add_executable(example-cm7-usb-mmc
        ${PLAY_SOURCES}
)

target_include_directories(example-cm7-usb-mmc PUBLIC
        ${INCLUDE_DIR}
)

target_link_libraries(example-cm7-usb-mmc PUBLIC
        -T${LD_FILE_CM7}
        core-stm32h745i-app-setup-setup1-cm7
        core-stm32h745i-app-support-cm7
        core-stm32h745i-lvgl-cm7
)
target_link_options(example-cm7-usb-mmc PRIVATE -Wl,-Map=example-cm7-usb-mmc.map,--cref)

add_custom_command(TARGET example-cm7-usb-mmc POST_BUILD
        COMMAND ${CMAKE_SIZE} $<TARGET_FILE:example-cm7-usb-mmc>
)

set(EXTERNAL_LOADER "${CMAKE_CURRENT_LIST_DIR}/MT25TL01G_STM32H745I-DISCO.stldr")


add_custom_target(FLASH_example-cm7-usb-mmc
        COMMAND STM32_Programmer_CLI --connect port=SWD -el "${EXTERNAL_LOADER}" --erase all --write ${CMAKE_BINARY_DIR}/example-cm7-usb-mmc.elf --verify --start
        DEPENDS ${example-cm7-usb-mmc}
)