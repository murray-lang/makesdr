cmake_minimum_required(VERSION 4.1)

project(core-stm32h745i-app-setup-example1-cm7)

set(LOCAL_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/include/stm32h745i/app/setup)

set(STARTUP_FILE "src/startup_stm32h745xihx.s")

set(CM7_APP_SOURCES
        ${LOCAL_INCLUDE_DIR}/config.h
        ${LOCAL_INCLUDE_DIR}/mpu_config.h
        ${LOCAL_INCLUDE_DIR}/request_lines.h
        ${LOCAL_INCLUDE_DIR}/stm32h7xx_it.h

        src/config.c
        src/mpu_config.c
        src/request_lines.cpp
        src/stm32h7xx_hal_msp.c
        src/stm32h7xx_it.c
        src/syscalls.c
        src/sysmem.c
        src/system_stm32h7xx_dualcore_boot_cm4_cm7.c
)

add_library(core-stm32h745i-app-setup-example1-cm7-startup OBJECT ${STARTUP_FILE})

add_library(core-stm32h745i-app-setup-example1-cm7 OBJECT ${CM7_APP_SOURCES})

target_compile_definitions(core-stm32h745i-app-setup-example1-cm7 PUBLIC CORE_CM7)

target_include_directories(core-stm32h745i-app-setup-example1-cm7 PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)

target_link_libraries(core-stm32h745i-app-setup-example1-cm7 PUBLIC

        $<TARGET_OBJECTS:core-stm32h745i-app-setup-example1-cm7-startup>
        core-stm32h745i-lvgl-cm7
        core-stm32h745i-app-support-cm7
)
