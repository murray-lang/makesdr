cmake_minimum_required(VERSION 4.1)

project(core-platform)

if(PLATFORM_FAMILY STREQUAL "stm32h745i")
    add_subdirectory(freertos)
    add_subdirectory(stm32h745i)
elseif(PLATFORM_FAMILY STREQUAL "linux")
    add_subdirectory(linux)
endif()