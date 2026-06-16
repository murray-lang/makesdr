cmake_minimum_required(VERSION 4.1)

project(core-stm32h745i-drivers)

add_subdirectory(conf)
add_subdirectory(CMSIS)
add_subdirectory(STM32H7xx_HAL_Driver)
add_subdirectory(BSP)