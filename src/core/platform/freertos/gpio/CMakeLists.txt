cmake_minimum_required(VERSION 4.1)

project(core-freertos-gpio)

set(FREERTOS_GPIO_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/include/gpio/service/Gpio.h
        ${CMAKE_CURRENT_LIST_DIR}/include/gpio/service/GpioInputLinesSource.h
        ${CMAKE_CURRENT_LIST_DIR}/include/gpio/service/GpioOutputLinesSource.h

        ${CMAKE_CURRENT_LIST_DIR}/src/Gpio.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/GpioInputLinesSource.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/GpioOutputLinesSource.cpp
)

add_library(core-freertos-gpio STATIC ${FREERTOS_GPIO_SOURCES})
target_include_directories(core-freertos-gpio PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)

if(TARGET_PLATFORM STREQUAL "stm32h745i-cm4")

    target_link_libraries(core-freertos-gpio PUBLIC
            core-gpio-base
            core-common-time
            core-common-thread
            core-freertos-thread
            core-stm32h745i-setup-setup3-cm4
            core-stm32h745i-drivers-hal-cm4
            etl::etl
    )
elseif(TARGET_PLATFORM STREQUAL "stm32h745i-cm7")

    target_link_libraries(core-freertos-gpio PUBLIC
            core-gpio-base
            core-common-time
            core-common-thread
            core-freertos-thread
            core-stm32h745i-app-setup-setup3-cm7
            core-stm32h745i-drivers-hal-cm7
            etl::etl
    )
endif()