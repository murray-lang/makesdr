cmake_minimum_required(VERSION 4.1)

project(core-freertos-usb-host)

set(FREERTOS_USB_HOST_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/include/usb/host/UsbHidHost.h
        ${CMAKE_CURRENT_LIST_DIR}/include/usb/host/UsbHost.h
        ${CMAKE_CURRENT_LIST_DIR}/src/UsbHidHost.cpp
        ${CMAKE_CURRENT_LIST_DIR}/include/usb/host/UsbHostThread.h

        ${CMAKE_CURRENT_LIST_DIR}/src/UsbHost.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/UsbHostThread.cpp
)

add_library(core-freertos-usb-host STATIC ${FREERTOS_USB_HOST_SOURCES})
target_include_directories(core-freertos-usb-host PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

# The USB host task must outrank the IQ/DSP thread (ADCIQ, priority 6), which can
# run flat out; at tskIDLE_PRIORITY it was starved and hub/device events were never
# processed. It mostly blocks in tuh_task_ext(), so this costs the DSP very little.
target_compile_definitions(core-freertos-usb-host PRIVATE
        USB_HOST_THREAD_PRIORITY=7
)

if(TARGET_PLATFORM STREQUAL "stm32h745i-cm7")
    target_link_libraries(core-freertos-usb-host PUBLIC
            core-usb-base
            core-stm32h745i-cm7-tinyusb-host
            core-freertos-thread
            etl::etl
    )
elseif(TARGET_PLATFORM STREQUAL "stm32h745i-cm4")
    target_link_libraries(core-freertos-usb-host PUBLIC
            core-usb-base
            core-stm32h745i-cm4-tinyusb-host
            core-freertos-thread
            etl::etl
            core-stm32h745i-drivers-bsp-disco-cm4
    )
endif()