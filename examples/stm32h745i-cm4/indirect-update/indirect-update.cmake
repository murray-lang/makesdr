cmake_minimum_required(VERSION 4.1)

project(example-cm4-indirect-update)


if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(DEBUG)
endif()

set(LD_FILE_CM4 ${CMAKE_CURRENT_LIST_DIR}/ld/STM32H745XIHX_FLASH.ld)

set(PLAY_SOURCES
#        ${CMAKE_CURRENT_LIST_DIR}/testSettings.h
#        ${CMAKE_CURRENT_LIST_DIR}/testSettings.cpp
        ${CMAKE_CURRENT_LIST_DIR}/main.cpp
#        ${CMAKE_CURRENT_LIST_DIR}/../../data/exampleRadioSettings.h
#        ${CMAKE_CURRENT_LIST_DIR}/../../data/exampleRadioSettings.cpp

        ${CMAKE_CURRENT_LIST_DIR}/gpioTest.cpp
        ${CMAKE_CURRENT_LIST_DIR}/gpioTest.h
        ${CMAKE_CURRENT_LIST_DIR}/digitalInputConfigs.cpp
        ${CMAKE_CURRENT_LIST_DIR}/digitalInputConfigs.h
)

add_executable(example-cm4-indirect-update
        ${PLAY_SOURCES}
)

target_include_directories(example-cm4-indirect-update PUBLIC
        ${INCLUDE_DIR}
)

target_link_libraries(example-cm4-indirect-update PUBLIC
        -T${LD_FILE_CM4}
        config-struct
        settings-model-core
        core-gpio-base
        core-freertos-gpio-cm4
        core-stm32h745i-setup-setup1-cm4
        core-stm32h745i-thread-cm4
        etl::etl
)

target_link_options(example-cm4-indirect-update PRIVATE -Wl,-Map=example-cm4-indirect-update.map,--cref)

add_custom_command(TARGET example-cm4-indirect-update POST_BUILD
        COMMAND ${CMAKE_SIZE} $<TARGET_FILE:example-cm4-indirect-update>
)

set(EXTERNAL_LOADER "${CMAKE_CURRENT_LIST_DIR}/MT25TL01G_STM32H745I-DISCO.stldr")

add_custom_target(FLASH_example-cm4-indirect-update
        COMMAND STM32_Programmer_CLI --connect port=SWD --write ${CMAKE_BINARY_DIR}/example-cm4-indirect-update.elf --start
        DEPENDS example-cm4-indirect-update
)