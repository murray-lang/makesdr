cmake_minimum_required(VERSION 4.1)

project(core-stm32h745i-tinyusb-device)

set(FAMILY stm32h7)

add_definitions(
        -DCFG_TUSB_MCU=OPT_MCU_STM32H7
        -DCFG_TUSB_CONFIG_FILE="tinyusb/device/tusb_config.h"
)

add_subdirectory(${TINYUSB_DIR} ${CMAKE_BINARY_DIR}/tinyusb-device)

set(STM32_TINYUSB_DEVICE_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/include/tinyusb/device/usb_device.h
        ${CMAKE_CURRENT_LIST_DIR}/include/tinyusb/device/tusb.h
        ${CMAKE_CURRENT_LIST_DIR}/include/tinyusb/device/tusb_config.h
        ${CMAKE_CURRENT_LIST_DIR}/include/tinyusb/device/usbd.h

        ${CMAKE_CURRENT_LIST_DIR}/src/usb_device.c
        ${CMAKE_CURRENT_LIST_DIR}/src/usb_descriptors.c
        ${CMAKE_CURRENT_LIST_DIR}/src/msc_disk.c
        # DCD driver for STM32H7 (uses DWC2 controller)
        ${TINYUSB_DIR}/portable/synopsys/dwc2/dcd_dwc2.c
        ${TINYUSB_DIR}/portable/synopsys/dwc2/dwc2_common.c
)

add_library(core-stm32h745i-cm4-tinyusb-device STATIC ${STM32_TINYUSB_DEVICE_SOURCES})
target_include_directories(core-stm32h745i-cm4-tinyusb-device PUBLIC include)

tinyusb_target_add(core-stm32h745i-cm4-tinyusb-device)

target_link_libraries(core-stm32h745i-cm4-tinyusb-device PUBLIC
        core-stm32h745i-drivers-bsp-disco-cm4
        core-stm32h745i-drivers-hal-cm4
)

add_library(core-stm32h745i-cm7-tinyusb-device STATIC ${STM32_TINYUSB_DEVICE_SOURCES})
target_include_directories(core-stm32h745i-cm7-tinyusb-device PUBLIC include)

tinyusb_target_add(core-stm32h745i-cm7-tinyusb-device)

target_link_libraries(core-stm32h745i-cm7-tinyusb-device PUBLIC
        core-stm32h745i-drivers-bsp-disco-cm7
        core-stm32h745i-drivers-hal-cm7

)

