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
        ${CMAKE_CURRENT_LIST_DIR}/Gpiod.cpp
        ${CMAKE_CURRENT_LIST_DIR}/GpioEventQueueGpiod.cpp
        ${CMAKE_CURRENT_LIST_DIR}/DigitalInputLinesRequestGpiod.cpp
        ${CMAKE_CURRENT_LIST_DIR}/DigitalOutputLinesRequestGpiod.cpp
)

add_library(core-linux-gpio STATIC ${GPIO_LINUX_SOURCES})

target_link_libraries(core-linux-gpio PUBLIC
        settings-control-hal-gpio
        PkgConfig::GPIOD
)
