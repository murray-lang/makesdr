cmake_minimum_required(VERSION 3.25)

project(core-sys-stm32h745i-freertos)

if(TARGET_PLATFORM STREQUAL "stm32h745i-cm7")
    set(FREERTOS_PORT GCC_ARM_CM7)
elseif (TARGET_PLATFORM STREQUAL "stm32h745i-cm4")
    set(FREERTOS_PORT GCC_ARM_CM4F)
endif()

# FreeRTOS looks for this specific target to find FreeRTOSConfig.h
add_library(freertos_config INTERFACE)
target_include_directories(freertos_config INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

set(FREERTOS_HEAP "4" CACHE STRING "" FORCE)

add_subdirectory(${FREERTOS_DIR} "${CMAKE_BINARY_DIR}/freertos-${TARGET_PLATFORM}")

set(DUMMY_FILE ${CMAKE_CURRENT_LIST_DIR}/dummy.c)

if(TARGET_PLATFORM STREQUAL "stm32h745i-cm7")
    add_library(core-sys-stm32h745i-freertos-cm7 STATIC ${DUMMY_FILE})
    target_link_libraries(core-sys-stm32h745i-freertos-cm7 PUBLIC
            freertos_config
            freertos_kernel
    )
elseif(TARGET_PLATFORM STREQUAL "stm32h745i-cm4")
    add_library(core-sys-stm32h745i-freertos-cm4 STATIC ${DUMMY_FILE})
    target_link_libraries(core-sys-stm32h745i-freertos-cm4 PUBLIC
            freertos_config
            freertos_kernel
    )
endif()