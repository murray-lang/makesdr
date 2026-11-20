cmake_minimum_required(VERSION 4.1)

project(core-stm32h745i-tinyusb-host)

set(FAMILY stm32h7)

add_definitions(
        -DCFG_TUSB_MCU=OPT_MCU_STM32H7
        -DCFG_TUSB_CONFIG_FILE="tinyusb/host/tusb_config.h"
        -DCFG_TUSB_OS_INC_PATH=${FREERTOS_DIR}/include/
)

add_subdirectory(${TINYUSB_DIR} ${CMAKE_BINARY_DIR}/tinyusb-host)

set(STM32_TINYUSB_HOST_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/include/tinyusb/host/usb_host.h
        ${CMAKE_CURRENT_LIST_DIR}/include/tinyusb/host/hid_host.h
        ${CMAKE_CURRENT_LIST_DIR}/include/tinyusb/host/tusb.h
        ${CMAKE_CURRENT_LIST_DIR}/include/tinyusb/host/tusb_config.h
        ${CMAKE_CURRENT_LIST_DIR}/include/tinyusb/host/usbh.h

        ${CMAKE_CURRENT_LIST_DIR}/src/usb_host.c

        # HCD driver for STM32H7 (uses DWC2 controller)
        ${TINYUSB_DIR}/portable/synopsys/dwc2/hcd_dwc2.c
        ${TINYUSB_DIR}/portable/synopsys/dwc2/dwc2_common.c
)

#add_library(core-stm32h745i-cm4-tinyusb-host-irq OBJECT ${CMAKE_CURRENT_LIST_DIR}/src/usbh_irq_handler.c)
#target_include_directories(core-stm32h745i-cm4-tinyusb-host-irq PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
#tinyusb_target_add(core-stm32h745i-cm4-tinyusb-host-irq)
#target_link_libraries(core-stm32h745i-cm4-tinyusb-host-irq PUBLIC
#        core-stm32h745i-cm4-freertos
#        core-stm32h745i-drivers-bsp-disco-cm4
#)


if(TARGET_PLATFORM STREQUAL "stm32h745i-cm4")
    add_library(core-stm32h745i-cm4-tinyusb-host STATIC ${STM32_TINYUSB_HOST_SOURCES})
    target_include_directories(core-stm32h745i-cm4-tinyusb-host PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)

    tinyusb_target_add(core-stm32h745i-cm4-tinyusb-host)

    ## Only for debugging with printf
    #target_sources(core-stm32h745i-cm4-tinyusb-host PRIVATE
    #        ${CMAKE_SOURCE_DIR}/src/core/platform/stm32h745i/app/setup/setup1/cm4/src/syscalls.c
    ##        ${CMAKE_SOURCE_DIR}/src/core/platform/stm32h745i/app/setup/setup1/cm4/src/config.c
    #)
    #
    ## Only for debugging with printf
    #target_include_directories(core-stm32h745i-cm4-tinyusb-host PRIVATE
    #        ${CMAKE_SOURCE_DIR}/src/core/platform/stm32h745i/app/setup/setup1/cm4/include
    #)

    target_link_libraries(core-stm32h745i-cm4-tinyusb-host PUBLIC
            core-stm32h745i-cm4-freertos
            core-stm32h745i-drivers-bsp-disco-cm4
            core-stm32h745i-drivers-hal-cm4
    #        $<TARGET_OBJECTS:core-stm32h745i-cm4-tinyusb-host-irq>
    )
endif()

if(TARGET_PLATFORM STREQUAL "stm32h745i-cm7")
    add_library(core-stm32h745i-cm7-tinyusb-host STATIC ${STM32_TINYUSB_HOST_SOURCES})
    target_include_directories(core-stm32h745i-cm7-tinyusb-host PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)

    tinyusb_target_add(core-stm32h745i-cm7-tinyusb-host)

    target_link_libraries(core-stm32h745i-cm7-tinyusb-host PUBLIC
            core-stm32h745i-cm7-freertos
            core-stm32h745i-drivers-bsp-disco-cm7
            core-stm32h745i-drivers-hal-cm7
    )
endif()
