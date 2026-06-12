cmake_minimum_required(VERSION 3.25)

project(core-sys-stm32h745i-app-common)

set(LOCAL_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/include/sys/stm32h745i/app/common)

set(STM32_APP_COMMON_SOURCES
        ${LOCAL_INCLUDE_DIR}/fatfs_wrapper.h
        ${LOCAL_INCLUDE_DIR}/misc_utils.h
        ${LOCAL_INCLUDE_DIR}/mmc_diskio.h
        ${LOCAL_INCLUDE_DIR}/mpu_config.h
        ${LOCAL_INCLUDE_DIR}/stm32h7xx_it.h
        ${LOCAL_INCLUDE_DIR}/semaphore.h
        ${LOCAL_INCLUDE_DIR}/usb_manager.h

        src/fatfs_wrapper.c
        src/misc_utils.c
        src/mmc_diskio.c
        src/mpu_config.c
        src/msc_disk.c
        src/syscalls.c
        src/sysmem.c
        src/system_stm32h7xx_dualcore_boot_cm4_cm7.c
        src/usb_manager.c
)

if(TARGET_PLATFORM STREQUAL "stm32h745i-cm7")

    add_library(core-sys-stm32h745i-app-common-cm7 STATIC ${STM32_APP_COMMON_SOURCES})

    target_compile_definitions(core-sys-stm32h745i-app-common-cm7 PUBLIC
            CORE_CM7
    )

    target_include_directories(core-sys-stm32h745i-app-common-cm7 PUBLIC
            ${CMAKE_CURRENT_LIST_DIR}/include
    )

    target_link_libraries(core-sys-stm32h745i-app-common-cm7 PUBLIC
            core-sys-stm32h745i-drivers-bsp-disco-cm7
            core-sys-stm32h745i-freertos-cm7
            core-sys-stm32h745i-tinyusb-cm7
            core-sys-stm32h745i-fatfs-cm7
    )
elseif(TARGET_PLATFORM STREQUAL "stm32h745i-cm4")

    add_library(core-sys-stm32h745i-app-common-cm4 STATIC ${STM32_APP_COMMON_SOURCES})

    target_compile_definitions(core-sys-stm32h745i-app-common-cm4 PUBLIC
            CORE_CM4
            DISABLE_MMC_INIT # CM7 is responsible for MMC init.
    )

    target_include_directories(core-sys-stm32h745i-app-common-cm4 PUBLIC
            ${CMAKE_CURRENT_LIST_DIR}/include
    )

    target_link_libraries(core-sys-stm32h745i-app-common-cm4 PUBLIC
            core-sys-stm32h745i-drivers-bsp-disco-cm4
            core-sys-stm32h745i-freertos-cm4
            core-sys-stm32h745i-tinyusb-cm4
            core-sys-stm32h745i-fatfs-cm4
    )
endif()

