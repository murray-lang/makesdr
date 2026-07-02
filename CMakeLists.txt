cmake_minimum_required(VERSION 4.1)


set_property(CACHE TARGET_PLATFORM PROPERTY STRINGS "linux-pc" "rpi" "stm32h745i-cm7" "stm32h745i-cm4")
set_property(CACHE TARGET_EXAMPLE PROPERTY STRINGS
        "play"
        "save"
        "restore"
        "qt-json-config"
        "qt-gpio-input"
        "indirect-update"
)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")

project(makesdr C CXX ASM)


# Derive platform family from TARGET_PLATFORM
if(TARGET_PLATFORM MATCHES "^stm32h745i-(cm[47])$")
    set(PLATFORM_FAMILY "stm32h745i")
    set(PLATFORM_VARIANT "${CMAKE_MATCH_1}")

    add_definitions(
            -DIS_STM32
            -DMAX_GPIO=27
            -DGPIO_EVENT_QUEUE_CAPACITY=32
            -DMAX_GPIO_CONTEXT_LENGTH=4
    )

elseif(TARGET_PLATFORM STREQUAL "rpi")
    set(PLATFORM_FAMILY "linux")
    set(PLATFORM_VARIANT "rpi")

    add_definitions(
            -DIS_LINUX
            -DMAX_GPIO=27
            -DGPIO_EVENT_QUEUE_CAPACITY=64
            -DMAX_GPIO_CONTEXT_LENGTH=16
    )

elseif(TARGET_PLATFORM STREQUAL "linux-pc")
    set(PLATFORM_FAMILY "linux")
    set(PLATFORM_VARIANT "pc")
    add_definitions(-DIS_LINUX)
endif()

if(NOT PLATFORM_FAMILY STREQUAL "linux")
    set(USE_FREERTOS ON)
    add_definitions(-DUSE_FREERTOS)
endif()

add_definitions(
        # These lengths are one less than the corresponding string sizes in the proto options file ('\0')

        -DMAX_NAME_LENGTH=7
        -DMAX_FIELD_PATH_LENGTH=8
        -DMAX_BAND_LABEL_LENGTH=9
        -DMAX_MODE_LABEL_LENGTH=14
        -DMAX_MODES=10
        -DMAX_ID_LENGTH=16
        -DMAX_DOTTED_STRING_LENGTH=64
        -DMAX_BAND_CATEGORIES=8
        -DMAX_BANDS_PER_CATEGORY=13

        -DMAX_GPIO_LINES_PER_DEVICE=4
        -DMAX_DIGITAL_INPUT_HANDLERS=8
        -DMAX_DIGITAL_OUTPUT_HANDLERS=4

        -DMAX_CONTROL_SINKS=4
        -DMAX_CONTROL_SOURCES=4


)

if(USE_ETL)
    add_definitions(-DUSE_ETL)
endif()

if(USE_GPIO)
    add_definitions(-DUSE_GPIO)
endif()

if(IS_QT)
    add_definitions(-DIS_QT)
endif()

set(THIRD_PARTY_DIR ${CMAKE_SOURCE_DIR}/third-party)
set(NANOPB_DIR ${THIRD_PARTY_DIR}/nanopb)
set(ETL_DIR ${THIRD_PARTY_DIR}/etl)
set(ARDUINO_JSON_DIR ${THIRD_PARTY_DIR}/ArduinoJson)
set(LVGL_DIR ${THIRD_PARTY_DIR}/lvgl)
set(TINYUSB_DIR ${THIRD_PARTY_DIR}/tinyusb_src/src)
set(FREERTOS_DIR ${THIRD_PARTY_DIR}/FreeRTOS-Kernel)
set(WEBSOCKETS_DIR ${THIRD_PARTY_DIR}/libwebsockets)

set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

# Note: Platform-specific compiler/linker flags are set in the toolchain files
# (arm-none-eabi-toolchain.cmake, linux-toolchain.cmake, etc.)

add_subdirectory(${NANOPB_DIR} nanopb)
add_subdirectory(${ARDUINO_JSON_DIR} arduinoJson)

add_subdirectory(${ETL_DIR} etl)
target_compile_definitions(etl INTERFACE ETL_NO_CHECKS) #Make sure that it can't throw exceptions

add_subdirectory(src/core)
add_subdirectory(src/settings)
add_subdirectory(src/config)

include(${CMAKE_SOURCE_DIR}/examples/${TARGET_PLATFORM}/${TARGET_EXAMPLE}/${TARGET_EXAMPLE}.cmake)

