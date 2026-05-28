cmake_minimum_required(VERSION 4.1)

project(RadioSettingsProto-settings-control-hal-usb)

if (TARGET_PLATFORM STREQUAL "linux")
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
endif()

set(USB_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/include/usb/UsbControl.h
        ${CMAKE_CURRENT_LIST_DIR}/include/usb/HidUsbControl.h
)

if (IS_LINUX)
    set(USB_SOURCES
            ${USB_SOURCES}
            ${CMAKE_CURRENT_LIST_DIR}/include/usb/LibUsbControl.h
            ${CMAKE_CURRENT_LIST_DIR}/platform/linux/HidUsbControl.cpp
            ${CMAKE_CURRENT_LIST_DIR}/platform/linux/LibUsbControl.cpp
    )
endif()

add_library(RadioSettingsProto-settings-control-hal-usb STATIC ${USB_SOURCES} )

if(IS_LINUX)
    target_link_libraries(RadioSettingsProto-settings-control-hal-usb PUBLIC usb-1.0 hidapi-libusb)
endif()

target_include_directories(RadioSettingsProto-settings-control-hal-usb PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
        /usr/include/hidapi/
)