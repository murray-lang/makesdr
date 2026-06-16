cmake_minimum_required(VERSION 4.1)

project(example-cm4-usb-mmc)


if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(DEBUG)
endif()

set(LD_FILE_CM4 ${CMAKE_CURRENT_LIST_DIR}/STM32H745XIHX_FLASH.ld)

set(CM4_USB_MMC_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/main.c
)

add_executable(example-cm4-usb-mmc ${CM4_USB_MMC_SOURCES} )

target_include_directories(example-cm4-usb-mmc PUBLIC
        ${INCLUDE_DIR}
)

target_link_libraries(example-cm4-usb-mmc PUBLIC
        -T${LD_FILE_CM4}
        core-stm32h745i-app-setup-example1-cm4
        core-stm32h745i-app-support-cm4
#        ArduinoJson
)

add_custom_command(TARGET example-cm4-usb-mmc POST_BUILD
        COMMAND ${CMAKE_SIZE} $<TARGET_FILE:example-cm4-usb-mmc>
)

set(EXTERNAL_LOADER "${CMAKE_CURRENT_LIST_DIR}/MT25TL01G_STM32H745I-DISCO.stldr")

add_custom_target(FLASH_CM4
        COMMAND STM32_Programmer_CLI --connect port=SWD --write ${CMAKE_BINARY_DIR}/example-cm4-usb-mmc.elf --start
        DEPENDS example-cm4-usb-mmc
)
