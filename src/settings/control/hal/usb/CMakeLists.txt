cmake_minimum_required(VERSION 4.1)

project(settings-control-hal-usb)

if (PLATFORM_FAMILY STREQUAL "linux")
    find_library(HIDAPI_LIBRARY hidapi-libusb)
    find_package(USB)
    if(NOT USB_FOUND)
        message(FATAL_ERROR "libusb required to compile nexusdr")
    endif()
    if(HIDAPI_LIBRARY)
        MESSAGE(STATUS "System Hidapi Lib ${HIDAPI_LIBRARY} is used")
    else()
        MESSAGE(FATAL_ERROR "Hidapi required to compile nexusdr")
    endif()

    add_definitions(-DLINUX_HIDAPI)
    set(IS_LINUX ON)
elseif (PLATFORM_FAMILY STREQUAL "stm32h745i")
    set(IS_STM32 ON) # This will be removed in favour of tinyusb **Don't forget AI work done on this in nexusdr!**
    set(STM32_USB_HOST_LIB_PATH "/home/murray/dev/code/stm32/disco/STM32Cube_FW_H7_V1.13.0/Middlewares/ST/STM32_USB_Host_Library")
endif()

set(USB_SOURCES
        include/settings/control/usb/UsbControl.h
        include/settings/control/usb/HidUsbControl.h
)

if (IS_LINUX)
    set(USB_SOURCES
            ${USB_SOURCES}
            include/settings/control/usb/LibUsbControl.h
            ${CMAKE_CURRENT_LIST_DIR}/platform/linux/HidUsbControl.cpp
            ${CMAKE_CURRENT_LIST_DIR}/platform/linux/LibUsbControl.cpp
    )
elseif (IS_STM32)
    set(USB_SOURCES
            ${USB_SOURCES}
            ${CMAKE_CURRENT_LIST_DIR}/platform/stm32/Stm32HidUsbControl.h
            ${CMAKE_CURRENT_LIST_DIR}/platform/stm32/Stm32HidUsbControl.cpp
            ${CMAKE_CURRENT_LIST_DIR}/platform/stm32/usbh_platform.cpp
            # STM32 USB Host Core
            ${STM32_USB_HOST_LIB_PATH}/Core/Src/usbh_core.c
            ${STM32_USB_HOST_LIB_PATH}/Core/Src/usbh_ctlreq.c
            ${STM32_USB_HOST_LIB_PATH}/Core/Src/usbh_ioreq.c
            ${STM32_USB_HOST_LIB_PATH}/Core/Src/usbh_pipes.c
            # STM32 USB Host HID Class
            ${STM32_USB_HOST_LIB_PATH}/Class/HID/Src/usbh_hid.c
            ${STM32_USB_HOST_LIB_PATH}/Class/HID/Src/usbh_hid_parser.c
    )
endif()

add_library(settings-control-hal-usb STATIC ${USB_SOURCES} )

if(IS_LINUX)
    target_link_libraries(settings-control-hal-usb PUBLIC usb-1.0 hidapi-libusb)
endif()

target_include_directories(settings-control-hal-usb PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

if (IS_LINUX)
    target_include_directories(settings-control-hal-usb PUBLIC
            /usr/include/hidapi/
    )
elseif (IS_STM32)
    target_include_directories(settings-control-hal-usb PUBLIC
            ${CMAKE_CURRENT_LIST_DIR}/platform/stm32
            ${STM32_USB_HOST_LIB_PATH}/Core/Inc
            ${STM32_USB_HOST_LIB_PATH}/Class/HID/Inc
    )
endif()