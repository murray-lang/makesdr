cmake_minimum_required(VERSION 4.1)


set_property(CACHE TARGET_PLATFORM PROPERTY STRINGS "linux-pc" "rpi" "stm32h745i-disco")
set_property(CACHE TARGET_EXAMPLE PROPERTY STRINGS
        "play"
        "save"
        "restore"
        "qt-json-config"
)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")

project(makesdr)

add_definitions(
        # These lengths are one less than the corresponding string sizes in the proto options file ('\0')

        -DMAX_NAME_LENGTH=7
        -DMAX_FIELD_PATH_LENGTH=6
        -DMAX_BAND_LABEL_LENGTH=9
        -DMAX_MODE_LABEL_LENGTH=14
        -DMAX_ID_LENGTH=16
        -DMAX_DOTTED_STRING_LENGTH=64
        -DMAX_BAND_CATEGORIES=8
        -DMAX_BANDS_PER_CATEGORY=13
        -DMAX_GPIO=27
        -DMAX_GPIO_LINES_PER_DEVICE=4
        -DMAX_DIGITAL_INPUT_HANDLERS=8
        -DMAX_DIGITAL_OUTPUT_HANDLERS=4
        -DMAX_DIGITAL_INPUT_LINES_TOTAL=10
        -DMAX_CONTROL_SINKS=4
        -DMAX_CONTROL_SOURCES=4
        -DMAX_GPIO_CONTEXT_LENGTH=16
        -DMAX_GPIO_EVENT_QUEUE_CAPACITY=64
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

set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

add_subdirectory(${ETL_DIR} etl)
add_subdirectory(${NANOPB_DIR} nanopb)
add_subdirectory(${ARDUINO_JSON_DIR} arduinoJson)

add_subdirectory(src/core)
add_subdirectory(src/settings)
add_subdirectory(src/config)

include(${CMAKE_SOURCE_DIR}/examples/${TARGET_PLATFORM}/${TARGET_EXAMPLE}/${TARGET_EXAMPLE}.cmake)

