cmake_minimum_required(VERSION 4.1)

project(makesdr-settings-control-hal)

if (USE_GPIO)
    add_subdirectory(gpio)
endif()

add_subdirectory(usb)
add_subdirectory(FunCubeDongle)