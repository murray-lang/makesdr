cmake_minimum_required(VERSION 4.1)

project(core-stm32h745i-setup-setup1-cm4)

set(LOCAL_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/include/stm32h745i/setup)

set(STARTUP_FILE "src/startup_stm32h745xihx.s")

set(CM4_APP_SOURCES
        ${LOCAL_INCLUDE_DIR}/config.h
        ${LOCAL_INCLUDE_DIR}/mpu_config.h
        ${LOCAL_INCLUDE_DIR}/gpio/GpioInputs.h
        ${LOCAL_INCLUDE_DIR}/stm32h7xx_it.h

        src/config.c
        src/mpu_config.c
        src/GpioInputs.cpp
        src/stm32h7xx_hal_msp.c
        src/stm32h7xx_it.c
        src/syscalls.c
        src/sysmem.c
        src/system_stm32h7xx_dualcore_boot_cm4_cm7.c
)

add_library(core-stm32h745i-setup-setup1-cm4-startup OBJECT ${STARTUP_FILE})

add_library(core-stm32h745i-setup-setup1-cm4 OBJECT ${CM4_APP_SOURCES})

target_compile_definitions(core-stm32h745i-setup-setup1-cm4 PUBLIC CORE_CM4)

target_include_directories(core-stm32h745i-setup-setup1-cm4 PUBLIC include)

target_link_libraries(core-stm32h745i-setup-setup1-cm4 PUBLIC
        core-gpio-base
        core-stm32h745i-app-support-cm4
        $<TARGET_OBJECTS:core-stm32h745i-setup-setup1-cm4-startup>
)
