cmake_minimum_required(VERSION 4.1)

project(core-stm32h745i-third-party)

add_subdirectory(tinyusb)
add_subdirectory(freertos)

if(TARGET_PLATFORM STREQUAL "stm32h745i-cm7")
    add_subdirectory(lvgl)
endif()
