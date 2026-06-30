cmake_minimum_required(VERSION 4.1)

project(core-freertos-gpio)

set(STM32_GPIO_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/include/gpio/service/GpioInputLinesSource.h
        ${CMAKE_CURRENT_LIST_DIR}/include/gpio/service/GpioOutputLinesSource.h
        ${CMAKE_CURRENT_LIST_DIR}/src/GpioInputLinesSource.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/GpioOutputLinesSource.cpp
)

if(TARGET_PLATFORM STREQUAL "stm32h745i-cm4")
    add_library(core-freertos-gpio-cm4 STATIC ${STM32_GPIO_SOURCES})
    target_include_directories(core-freertos-gpio-cm4 PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)

    target_link_libraries(core-freertos-gpio-cm4 PUBLIC
            core-gpio-base
            core-stm32h745i-setup-setup1-cm4
            core-stm32h745i-freertos-cm4
            core-stm32h745i-thread-cm4
            core-stm32h745i-drivers-hal-cm4
            etl::etl
    )
elseif(TARGET_PLATFORM STREQUAL "stm32h745i-cm7")
    add_library(core-stm32h745i-gpio-cm7 STATIC ${STM32_GPIO_SOURCES})
    target_include_directories(core-stm32h745i-gpio-cm7 PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)

    target_link_libraries(core-stm32h745i-gpio-cm7 PUBLIC
            core-gpio-base
            core-stm32h745i-app-setup-setup1-cm7
            core-stm32h745i-freertos-cm4
            core-stm32h745i-thread-cm7
            core-stm32h745i-drivers-hal-cm7
            etl::etl
    )
endif()