cmake_minimum_required(VERSION 4.1)

project(core-stm32h745i-app-support)

set(LOCAL_INCLUDE_DIR include/stm32h745i/app/support)

set(STM32_APP_COMMON_SOURCES
        ${LOCAL_INCLUDE_DIR}/fatfs_wrapper.h
        ${LOCAL_INCLUDE_DIR}/safe_printf.h
        ${LOCAL_INCLUDE_DIR}/mmc_diskio.h
        ${LOCAL_INCLUDE_DIR}/semaphore.h
        ${LOCAL_INCLUDE_DIR}/usb_manager.h
        ${LOCAL_INCLUDE_DIR}/usb_msc.h

        src/fatfs_wrapper.c
        src/mmc_diskio.c
        src/usb_manager.c
        src/usb_msc.c
)

if(TARGET_PLATFORM STREQUAL "stm32h745i-cm7")
    add_library(core-stm32h745i-app-support-cm7 STATIC ${STM32_APP_COMMON_SOURCES})

    target_compile_definitions(core-stm32h745i-app-support-cm7 PUBLIC
            CORE_CM7
    )

    target_include_directories(core-stm32h745i-app-support-cm7 PUBLIC
            ${CMAKE_CURRENT_LIST_DIR}/include
    )

    target_link_libraries(core-stm32h745i-app-support-cm7 PUBLIC
            core-stm32h745i-drivers-bsp-disco-cm7
            core-stm32h745i-freertos-cm7
            core-stm32h745i-tinyusb-cm7
            core-stm32h745i-fatfs-cm7
    )
elseif(TARGET_PLATFORM STREQUAL "stm32h745i-cm4")
    add_library(core-stm32h745i-app-support-cm4 STATIC ${STM32_APP_COMMON_SOURCES})

    target_compile_definitions(core-stm32h745i-app-support-cm4 PUBLIC
            CORE_CM4
            DISABLE_MMC_INIT # CM7 is responsible for MMC init.
    )

    target_include_directories(core-stm32h745i-app-support-cm4 PUBLIC
            ${CMAKE_CURRENT_LIST_DIR}/include
    )

    target_link_libraries(core-stm32h745i-app-support-cm4 PUBLIC
            core-stm32h745i-drivers-bsp-disco-cm4
            core-stm32h745i-freertos-cm4
            core-stm32h745i-tinyusb-cm4
            core-stm32h745i-fatfs-cm4
    )
endif()

