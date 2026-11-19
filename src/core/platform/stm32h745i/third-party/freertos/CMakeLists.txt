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

#set(FREERTOS_HEAP "4" CACHE STRING "" FORCE)
# liquid-dsp probably needs heap_3.c because it might call realloc
set(FREERTOS_HEAP "3" CACHE STRING "" FORCE)

add_subdirectory(${FREERTOS_DIR} "${CMAKE_BINARY_DIR}/freertos-${TARGET_PLATFORM}")

set(DUMMY_FILE dummy.c)

if(TARGET_PLATFORM STREQUAL "stm32h745i-cm7")
    add_library(core-stm32h745i-cm7-freertos STATIC ${DUMMY_FILE})
    target_link_libraries(core-stm32h745i-cm7-freertos PUBLIC
            freertos_config
            freertos_kernel
    )
elseif(TARGET_PLATFORM STREQUAL "stm32h745i-cm4")
    add_library(core-stm32h745i-cm4-freertos STATIC ${DUMMY_FILE})
    target_link_libraries(core-stm32h745i-cm4-freertos PUBLIC
            freertos_config
            freertos_kernel
    )
endif()