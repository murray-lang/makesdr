cmake_minimum_required(VERSION 3.25)

project(core-sys-stm32h745i-tinyusb)

set(FAMILY stm32h7)

add_definitions( -DCFG_TUSB_MCU=OPT_MCU_STM32H7 )

add_subdirectory(${TINYUSB_DIR} ${CMAKE_BINARY_DIR}/tinyusb)

set(STM32_TINYUSB_SOURCES
        include/sys/stm32h745i/tinyusb/tusb.h
        include/sys/stm32h745i/tinyusb/tusb_config.h
        ${CMAKE_CURRENT_LIST_DIR}/usb_descriptors.c
)

add_library(core-sys-stm32h745i-tinyusb-cm4 STATIC ${STM32_TINYUSB_SOURCES})
target_include_directories(core-sys-stm32h745i-tinyusb-cm4 PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)

tinyusb_target_add(core-sys-stm32h745i-tinyusb-cm4)

target_link_libraries(core-sys-stm32h745i-tinyusb-cm4 PUBLIC
        core-sys-stm32h745i-drivers-bsp-disco-cm4
        core-sys-stm32h745i-drivers-hal-cm4
)

add_library(core-sys-stm32h745i-tinyusb-cm7 STATIC ${STM32_TINYUSB_SOURCES})
target_include_directories(core-sys-stm32h745i-tinyusb-cm7 PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)

tinyusb_target_add(core-sys-stm32h745i-tinyusb-cm7)

target_link_libraries(core-sys-stm32h745i-tinyusb-cm7 PUBLIC
        core-sys-stm32h745i-drivers-bsp-disco-cm7
        core-sys-stm32h745i-drivers-hal-cm7
)

