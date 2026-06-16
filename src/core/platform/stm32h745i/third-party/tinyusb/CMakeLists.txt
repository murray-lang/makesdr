cmake_minimum_required(VERSION 4.1)

project(core-stm32h745i-tinyusb)

set(FAMILY stm32h7)

add_definitions(
        -DCFG_TUSB_MCU=OPT_MCU_STM32H7
        -DCFG_TUSB_CONFIG_FILE="stm32h745i/tinyusb/tusb_config.h"
)

add_subdirectory(${TINYUSB_DIR} ${CMAKE_BINARY_DIR}/tinyusb)

set(STM32_TINYUSB_SOURCES
        include/stm32h745i/tinyusb/tusb.h
        include/stm32h745i/tinyusb/tusb_config.h
        include/stm32h745i/tinyusb/device/usbd.h
        usb_descriptors.c
        msc_disk.c
        # DCD driver for STM32H7 (uses DWC2 controller)
        ${TINYUSB_DIR}/portable/synopsys/dwc2/dcd_dwc2.c
        ${TINYUSB_DIR}/portable/synopsys/dwc2/dwc2_common.c
)

add_library(core-stm32h745i-tinyusb-cm4 STATIC ${STM32_TINYUSB_SOURCES})
target_include_directories(core-stm32h745i-tinyusb-cm4 PUBLIC include)

tinyusb_target_add(core-stm32h745i-tinyusb-cm4)

target_link_libraries(core-stm32h745i-tinyusb-cm4 PUBLIC
        core-stm32h745i-drivers-bsp-disco-cm4
        core-stm32h745i-drivers-hal-cm4
)

add_library(core-stm32h745i-tinyusb-cm7 STATIC ${STM32_TINYUSB_SOURCES})
target_include_directories(core-stm32h745i-tinyusb-cm7 PUBLIC include)

tinyusb_target_add(core-stm32h745i-tinyusb-cm7)

target_link_libraries(core-stm32h745i-tinyusb-cm7 PUBLIC
        core-stm32h745i-drivers-bsp-disco-cm7
        core-stm32h745i-drivers-hal-cm7
)

