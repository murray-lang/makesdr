cmake_minimum_required(VERSION 4.1)

project(radios-iq-single-rx-transport-stm32h745i-globals)

set(STM32H745_TRANSPORT_GLOBALS_IQ_SINGLE_RX_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/stm32h745i/globalStm32H745SingleIqRxRings.h
)

add_library(radios-iq-single-rx-transport-stm32h745i-globals INTERFACE ${STM32H745_TRANSPORT_GLOBALS_IQ_SINGLE_RX_FILES})

target_include_directories(radios-iq-single-rx-transport-stm32h745i-globals INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(radios-iq-single-rx-transport-stm32h745i-globals INTERFACE
        core-stm32h745i-transport
        settings-model-radio-iq-single-rx
)
