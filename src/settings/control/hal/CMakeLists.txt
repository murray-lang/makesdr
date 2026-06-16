cmake_minimum_required(VERSION 4.1)

project(settings-control-hal)

if (PLATFORM_FAMILY STREQUAL "linux") # Deal with other platforms properly as they are implemented
    if (USE_GPIO)
        add_subdirectory(gpio)
    endif()

    add_subdirectory(usb)
    add_subdirectory(FunCubeDongle)
endif()