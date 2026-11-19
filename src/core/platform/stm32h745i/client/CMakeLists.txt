cmake_minimum_required(VERSION 4.1)

project(core-stm32h745i-client)

set(FREERTOS_CLIENT_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/stm32h745i/client/RadioClient.h
        ${CMAKE_CURRENT_LIST_DIR}/src/RadioClient.cpp
)

add_library(core-stm32h745i-client STATIC ${FREERTOS_CLIENT_FILES})

target_include_directories(core-stm32h745i-client PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

set(FREERTOS_CLIENT_LIBS
        core-freertos-transport-client
        settings-model-radio-base
        radios-iq-single-rx-transport-stm32h745i-globals
        radios-base
)

# For debugging with BSP_LED_*()
if(TARGET_PLATFORM STREQUAL "stm32h745i-cm7")
    list(APPEND FREERTOS_CLIENT_LIBS core-stm32h745i-drivers-bsp-disco-cm7)
elseif(TARGET_PLATFORM STREQUAL "stm32h745i-cm4")
    list(APPEND FREERTOS_CLIENT_LIBS core-stm32h745i-drivers-bsp-disco-cm4)
endif()

target_link_libraries(core-stm32h745i-client PUBLIC
        ${FREERTOS_CLIENT_LIBS}
)

