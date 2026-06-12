cmake_minimum_required(VERSION 3.25)

project(example-cm7-indirect-update)

# Example-specific compile definitions
add_compile_definitions(
    STM32H745xx
    USE_STM32H745I_DISCO
    USE_HAL_DRIVER
    USE_PWR_DIRECT_SMPS_SUPPLY
    CORE_CM7
    TS_MULTI_TOUCH_SUPPORTED
    LV_DRV_NO_CONF
    LV_CONF_INCLUDE_SIMPLE
    LIMIT_FATFS_CLOCK_FOR_CM4   # Slow down SDMMC clock for CM4 FatFs access
    CFG_TUSB_MCU=OPT_MCU_STM32H7
    HSE_VALUE=25000000
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(DEBUG)
endif()

set(PLAY_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/main.cpp
        ${CMAKE_CURRENT_LIST_DIR}/sys/src/syscalls.c
        ${CMAKE_CURRENT_LIST_DIR}/sys/src/sysmem.c
        ${CMAKE_CURRENT_LIST_DIR}/sys/src/system_stm32h7xx_dualcore_boot_cm4_cm7.c
        ${CMAKE_CURRENT_LIST_DIR}/../../data/exampleRadioSettings.h
        ${CMAKE_CURRENT_LIST_DIR}/../../data/exampleRadioSettings.cpp
)

set(STARTUP_FILE "${CMAKE_CURRENT_LIST_DIR}/sys/src/startup_stm32h745xihx.s")
set(LD_FILE ${CMAKE_CURRENT_LIST_DIR}/ld/STM32H745XIHX_QSPI.ld)

add_library(CM7_STARTUP_OBJ OBJECT ${STARTUP_FILE})

add_executable(example-cm7-indirect-update
     ${PLAY_SOURCES}
        $<TARGET_OBJECTS:CM7_STARTUP_OBJ>
)

target_include_directories(example-cm7-indirect-update PUBLIC
        ${INCLUDE_DIR}
)

target_link_libraries(example-cm7-indirect-update PUBLIC
        -T${LD_FILE}
        -Wl,-Map=${TARGET_CM7}.map
        --specs=nosys.specs
        -Wl,--gc-sections
        -Wl,--start-group -lc -lm -Wl,--end-group
        core-sys-stm32h745i-drivers-bsp-disco-cm7
        settings-model-proto
        settings-model-core
        settings-model-meta
)

add_custom_command(TARGET example-cm7-indirect-update POST_BUILD
        COMMAND ${CMAKE_SIZE} $<TARGET_FILE:example-cm7-indirect-update>
)