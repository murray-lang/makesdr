cmake_minimum_required(VERSION 3.25)

project(core-sys-stm32h745i)

add_definitions(
        -DSTM32H745xx
        -DUSE_STM32H745I_DISCO
        -DUSE_HAL_DRIVER
        -DUSE_PWR_DIRECT_SMPS_SUPPLY
        -DLIMIT_FATFS_CLOCK_FOR_CM4
        -DHSE_VALUE=25000000
)

add_subdirectory(drivers)
add_subdirectory(middlewares/third-party/FatFs)
add_subdirectory(tinyusb)
add_subdirectory(freertos)

if(TARGET_PLATFORM STREQUAL "stm32h745i-cm7")
    add_subdirectory(lvgl)
endif()

add_subdirectory(app)


