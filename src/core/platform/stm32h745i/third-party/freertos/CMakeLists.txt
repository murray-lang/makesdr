cmake_minimum_required(VERSION 4.1)

project(core-stm32h745i-freertos)

if(TARGET_PLATFORM STREQUAL "stm32h745i-cm7")
    set(FREERTOS_PORT GCC_ARM_CM7)
elseif (TARGET_PLATFORM STREQUAL "stm32h745i-cm4")
    set(FREERTOS_PORT GCC_ARM_CM4F)
endif()

# FreeRTOS looks for this specific target to find FreeRTOSConfig.h
add_library(freertos_config INTERFACE)
target_include_directories(freertos_config INTERFACE include)

set(FREERTOS_HEAP "4" CACHE STRING "" FORCE)

add_subdirectory(${FREERTOS_DIR} "${CMAKE_BINARY_DIR}/freertos-${TARGET_PLATFORM}")

set(DUMMY_FILE dummy.c)

if(TARGET_PLATFORM STREQUAL "stm32h745i-cm7")
    add_library(core-stm32h745i-freertos-cm7 STATIC ${DUMMY_FILE})
    target_link_libraries(core-stm32h745i-freertos-cm7 PUBLIC
            freertos_config
            freertos_kernel
    )
elseif(TARGET_PLATFORM STREQUAL "stm32h745i-cm4")
    add_library(core-stm32h745i-freertos-cm4 STATIC ${DUMMY_FILE})
    target_link_libraries(core-stm32h745i-freertos-cm4 PUBLIC
            freertos_config
            freertos_kernel
    )
endif()