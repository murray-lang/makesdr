cmake_minimum_required(VERSION 4.1)

project(core-stm32h745i-app-setup)

if(TARGET_PLATFORM STREQUAL "stm32h745i-cm4")
    add_subdirectory(example1/cm4)
endif()

if(TARGET_PLATFORM STREQUAL "stm32h745i-cm7")
    add_subdirectory(example1/cm7)
endif()