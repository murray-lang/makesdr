cmake_minimum_required(VERSION 4.1)

project(core-stm32h745i)

add_definitions(
        -DSTM32H745xx
        -DUSE_STM32H745I_DISCO
        -DUSE_HAL_DRIVER
        -DUSE_PWR_DIRECT_SMPS_SUPPLY
        -DLIMIT_FATFS_CLOCK_FOR_CM4
        -DHSE_VALUE=25000000
)

add_subdirectory(ST)
add_subdirectory(time)
add_subdirectory(third-party)
add_subdirectory(app)
