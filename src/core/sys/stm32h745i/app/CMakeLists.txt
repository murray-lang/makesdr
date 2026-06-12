cmake_minimum_required(VERSION 3.25)

project(core-sys-stm32h745i-app)

add_subdirectory(common)

if(TARGET_PLATFORM STREQUAL "stm32h745i-cm4")
    add_subdirectory(cm4)
endif()

if(TARGET_PLATFORM STREQUAL "stm32h745i-cm7")
    add_subdirectory(cm7)
endif()
