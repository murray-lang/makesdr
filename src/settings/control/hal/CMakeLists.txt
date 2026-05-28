cmake_minimum_required(VERSION 4.1)

project(RadioSettingsProto-settings-control-hal)

if (NOT TARGET_PLATFORM STREQUAL "linux")
    add_subdirectory(gpio)
endif()

add_subdirectory(usb)
add_subdirectory(FunCubeDongle)