cmake_minimum_required(VERSION 4.1)

project(core-linux-gpio)

list(APPEND CMAKE_PREFIX_PATH "/usr/lib/x86_64-linux-gnu/cmake")

find_package(PkgConfig)

pkg_check_modules(GPIOD IMPORTED_TARGET libgpiod)
if(GPIOD_FOUND)
    message(STATUS "libgpiod found")
else()
    message(FATAL_ERROR "libgpiod not found")
endif()

set(GPIO_LINUX_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/include/gpio/service/Gpio.h
        ${CMAKE_CURRENT_LIST_DIR}/include/gpio/service/GpioLinesSourceBase.h
        ${CMAKE_CURRENT_LIST_DIR}/include/gpio/service/GpioInputLinesSource.h
        ${CMAKE_CURRENT_LIST_DIR}/include/gpio/service/GpioOutputLinesSource.h

        ${CMAKE_CURRENT_LIST_DIR}/src/Gpio.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/GpioInputLinesSource.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/GpioOutputLinesSource.cpp

#        src/Gpiod.cpp
#        src/GpioEventQueueGpiod.cpp
#        src/DigitalInputLinesRequestGpiod.cpp
#        src/DigitalOutputLinesRequestGpiod.cpp
)

add_library(core-linux-gpio STATIC ${GPIO_LINUX_SOURCES})

target_include_directories(core-linux-gpio PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)

target_link_libraries(core-linux-gpio PUBLIC
        core-gpio-base
        core-linux-time
        core-linux-thread
        PkgConfig::GPIOD
)
