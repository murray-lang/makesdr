cmake_minimum_required(VERSION 4.1)

project(core-stm32h745i-setup-setup3-cm4)

set(STARTUP_FILE "cubemx/Core/Startup/startup_stm32h745xihx.s")
add_library(core-stm32h745i-setup-setup3-cm4-startup OBJECT ${STARTUP_FILE})

file(GLOB_RECURSE CM4_CUBEMX_HEADERS ${CMAKE_CURRENT_LIST_DIR}/cubemx/*.h)
file(GLOB_RECURSE CM4_CUBEMX_SOURCES ${CMAKE_CURRENT_LIST_DIR}/cubemx/*.c)
list(REMOVE_ITEM CM4_CUBEMX_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/cubemx/Core/Inc/stm32h7xx_hal_conf.h
        ${CMAKE_CURRENT_LIST_DIR}/cubemx/Core/Src/stm32h7xx_it.c
        ${CMAKE_CURRENT_LIST_DIR}/cubemx/Core/Src/sai.c
        ${CMAKE_CURRENT_LIST_DIR}/cubemx/Core/Src/i2c.c
)

set(MAKESDR_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/makesdr/include/stm32h745i)

set(CM4_SETUP_SOURCES
        ${CM4_CUBEMX_HEADERS}
        ${CM4_CUBEMX_SOURCES}

        ${MAKESDR_INCLUDE_DIR}/AdcConfig.h
        ${MAKESDR_INCLUDE_DIR}/AdcConfig_private.h
        ${MAKESDR_INCLUDE_DIR}/config.h
        ${MAKESDR_INCLUDE_DIR}/mpu_config.h
        ${MAKESDR_INCLUDE_DIR}/stm32h7xx_it.h
        ${MAKESDR_INCLUDE_DIR}/gpio/GpioInputs.h
        ${MAKESDR_INCLUDE_DIR}/gpio/GpioOutputs.h

        ${CMAKE_CURRENT_LIST_DIR}/makesdr/src/AdcConfig.c
        ${CMAKE_CURRENT_LIST_DIR}/makesdr/src/config.c
        ${CMAKE_CURRENT_LIST_DIR}/makesdr/src/stm32h7xx_it.c
        ${CMAKE_CURRENT_LIST_DIR}/makesdr/src/GpioInputs.cpp
        ${CMAKE_CURRENT_LIST_DIR}/makesdr/src/GpioOutputs.cpp
        ${CMAKE_CURRENT_LIST_DIR}/makesdr/src/mpu_config.c
)

add_library(core-stm32h745i-setup-setup3-cm4 OBJECT ${CM4_SETUP_SOURCES})

target_compile_definitions(core-stm32h745i-setup-setup3-cm4 PUBLIC CORE_CM4)

target_include_directories(core-stm32h745i-setup-setup3-cm4 PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/makesdr/include
        ${CMAKE_CURRENT_LIST_DIR}/cubemx/Core/Inc
)

target_link_libraries(core-stm32h745i-setup-setup3-cm4 PUBLIC
        core-gpio-base
        $<TARGET_OBJECTS:core-stm32h745i-setup-setup3-cm4-startup>
        core-stm32h745i-app-support-cm4
)
