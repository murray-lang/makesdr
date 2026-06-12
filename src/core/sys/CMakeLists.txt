cmake_minimum_required(VERSION 3.25)

project(core-sys)

if(PLATFORM_FAMILY STREQUAL "stm32h745i")
    add_subdirectory(stm32h745i)
endif()