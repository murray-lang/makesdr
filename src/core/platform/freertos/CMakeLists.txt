cmake_minimum_required(VERSION 4.1)

project(core-freertos)

# Tell ETL to use FreeRTOS mutex implementation for this platform
add_compile_definitions(ETL_TARGET_OS_FREERTOS=1)

add_subdirectory(gpio)
add_subdirectory(third-party)
add_subdirectory(thread)
add_subdirectory(transport)
add_subdirectory(usb)
