cmake_minimum_required(VERSION 3.25)

project(core-sys-stm32h745i-drivers)

add_subdirectory(conf)
add_subdirectory(CMSIS)
add_subdirectory(STM32H7xx_HAL_Driver)
add_subdirectory(BSP)