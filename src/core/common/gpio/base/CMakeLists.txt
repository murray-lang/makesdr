cmake_minimum_required(VERSION 4.1)

project(core-gpio-base)

set(GPIO_BASE_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/include/gpio/base/GpioLineMask.h
        ${CMAKE_CURRENT_LIST_DIR}/include/gpio/base/GpioLines.h
        ${CMAKE_CURRENT_LIST_DIR}/include/gpio/base/GpioLineValue.h
        ${CMAKE_CURRENT_LIST_DIR}/include/gpio/input/GpioInputLines.h
        ${CMAKE_CURRENT_LIST_DIR}/include/gpio/input/GpioInputLinesRequest.h
        ${CMAKE_CURRENT_LIST_DIR}/include/gpio/input/GpioLineEvent.h
        ${CMAKE_CURRENT_LIST_DIR}/include/gpio/input/GpioLineEventCallback.h
        ${CMAKE_CURRENT_LIST_DIR}/include/gpio/input/GpioLineReader.h
        ${CMAKE_CURRENT_LIST_DIR}/include/gpio/input/handlers/GpioLineDebouncer.h
        ${CMAKE_CURRENT_LIST_DIR}/include/gpio/input/handlers/GpioLineTransitionHandler.h
        ${CMAKE_CURRENT_LIST_DIR}/include/gpio/input/handlers/GpioLineTransitionHandlerVariant.h
        ${CMAKE_CURRENT_LIST_DIR}/include/gpio/input/handlers/GpioRotaryEncoder.h
        ${CMAKE_CURRENT_LIST_DIR}/include/gpio/input/handlers/GpioTransitionHandlers.h
        ${CMAKE_CURRENT_LIST_DIR}/include/gpio/output/GpioLineWriter.h
        ${CMAKE_CURRENT_LIST_DIR}/include/gpio/output/GpioOutputLinesRequest.h


        ${CMAKE_CURRENT_LIST_DIR}/src/GpioInputLines.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/GpioLineDebouncer.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/GpioLines.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/GpioRotaryEncoder.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/GpioTransitionHandlers.cpp
)

add_library(core-gpio-base STATIC ${GPIO_BASE_SOURCES})
target_compile_definitions(core-gpio-base PUBLIC
        TIMESTAMP_WIDTH=$<IF:$<STREQUAL:${PLATFORM_FAMILY},linux>,64,32>
        LINE_MASK_WIDTH=$<IF:$<STREQUAL:${PLATFORM_FAMILY},stm32h745i>,16,32>
)

target_include_directories(core-gpio-base PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)

target_link_libraries(core-gpio-base PUBLIC
        config-struct
        core-common-time
#        core-stm32h745i-app-support-cm4
#        core-stm32h745i-drivers-bsp-disco-cm4
        etl::etl
)
