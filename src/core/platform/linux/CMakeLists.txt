cmake_minimum_required(VERSION 4.1)

project(core-linux)

add_subdirectory(audio)
add_subdirectory(event)
add_subdirectory(iq)

if(USE_GPIO)
    add_subdirectory(gpio)
endif()
add_subdirectory(usb)
add_subdirectory(third-party)
add_subdirectory(thread)
add_subdirectory(time)
add_subdirectory(transport)
add_subdirectory(test-utils)
