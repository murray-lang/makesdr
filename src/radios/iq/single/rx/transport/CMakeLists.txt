cmake_minimum_required(VERSION 4.1)

add_subdirectory(qt)

if(PLATFORM_FAMILY STREQUAL "stm32h745i")
    add_subdirectory(stm32h745i)
endif()
