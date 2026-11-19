cmake_minimum_required(VERSION 4.1)

project(core-stm32h745i-setup-setup3-cm7)

set(STARTUP_FILE "cubemx/Core/Startup/startup_stm32h745xihx.s")
add_library(core-stm32h745i-setup-setup3-cm7-startup OBJECT ${STARTUP_FILE})

file(GLOB_RECURSE CM7_CUBEMX_HEADERS ${CMAKE_CURRENT_LIST_DIR}/cubemx/*.h)
file(GLOB_RECURSE CM7_CUBEMX_SOURCES ${CMAKE_CURRENT_LIST_DIR}/cubemx/*.c)
list(REMOVE_ITEM CM7_CUBEMX_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/cubemx/Core/Src/stm32h7xx_hal_msp.c
        ${CMAKE_CURRENT_LIST_DIR}/cubemx/Core/Src/stm32h7xx_it.c
)

set(MAKESDR_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/makesdr/include/stm32h745i)

set(CM7_SETUP_SOURCES
        ${CM7_CUBEMX_HEADERS}
        ${CM7_CUBEMX_SOURCES}

        ${MAKESDR_INCLUDE_DIR}/config.h
        ${MAKESDR_INCLUDE_DIR}/mpu_config.h
        ${MAKESDR_INCLUDE_DIR}/request_lines.h
        ${MAKESDR_INCLUDE_DIR}/stm32h7xx_it.h

        ${CMAKE_CURRENT_LIST_DIR}/makesdr/src/config.c
        ${CMAKE_CURRENT_LIST_DIR}/makesdr/src/mpu_config.c
        ${CMAKE_CURRENT_LIST_DIR}/makesdr/src/request_lines.cpp
        ${CMAKE_CURRENT_LIST_DIR}/makesdr/src/stm32h7xx_hal_msp.c
        ${CMAKE_CURRENT_LIST_DIR}/makesdr/src/stm32h7xx_it.c
)


add_library(core-stm32h745i-setup-setup3-cm7 OBJECT ${CM7_SETUP_SOURCES})

target_compile_definitions(core-stm32h745i-setup-setup3-cm7 PUBLIC CORE_CM7)

target_include_directories(core-stm32h745i-setup-setup3-cm7 PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/makesdr/include
        ${CMAKE_CURRENT_LIST_DIR}/cubemx/Core/Inc
)

target_link_libraries(core-stm32h745i-setup-setup3-cm7 PUBLIC
        $<TARGET_OBJECTS:core-stm32h745i-setup-setup3-cm7-startup>
        core-stm32h745i-lvgl-cm7
        core-stm32h745i-app-support-cm7
)
