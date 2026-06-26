cmake_minimum_required(VERSION 4.1)

project(core-gpio-base)

set(GPIO_CONFIG_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/include/gpio/base/GpioInputLines.h
        ${CMAKE_CURRENT_LIST_DIR}/include/gpio/base/GpioInputLinesRequest.h
        ${CMAKE_CURRENT_LIST_DIR}/include/gpio/base/GpioLineEvent.h
        ${CMAKE_CURRENT_LIST_DIR}/include/gpio/base/GpioLineEventCallback.h
        ${CMAKE_CURRENT_LIST_DIR}/include/gpio/base/GpioLineMask.h
        ${CMAKE_CURRENT_LIST_DIR}/include/gpio/base/GpioLineReader.h
        ${CMAKE_CURRENT_LIST_DIR}/include/gpio/base/GpioLineReaderDelegate.h
        ${CMAKE_CURRENT_LIST_DIR}/include/gpio/base/GpioLines.h
        ${CMAKE_CURRENT_LIST_DIR}/include/gpio/base/GpioLineValue.h
        ${CMAKE_CURRENT_LIST_DIR}/include/gpio/base/GpioCheckedLineReader.h
        ${CMAKE_CURRENT_LIST_DIR}/include/gpio/base/GpioNowGetter.h
        ${CMAKE_CURRENT_LIST_DIR}/include/gpio/base/GpioTimestamp.h

        ${CMAKE_CURRENT_LIST_DIR}/include/gpio/handlers/GpioLineDebouncer.h
        ${CMAKE_CURRENT_LIST_DIR}/include/gpio/handlers/GpioLineTransitionHandler.h
        ${CMAKE_CURRENT_LIST_DIR}/include/gpio/handlers/GpioLineTransitionHandlerVariant.h
        ${CMAKE_CURRENT_LIST_DIR}/include/gpio/handlers/GpioRotaryEncoder.h
        ${CMAKE_CURRENT_LIST_DIR}/include/gpio/handlers/GpioTransitionHandlers.h

        ${CMAKE_CURRENT_LIST_DIR}/src/GpioLineDebouncer.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/GpioLines.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/GpioRotaryEncoder.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/GpioTransitionHandlers.cpp
)

add_library(core-gpio-base STATIC ${GPIO_CONFIG_SOURCES})
target_compile_definitions(core-gpio-base PUBLIC
        TIMESTAMP_WIDTH=$<IF:$<STREQUAL:${PLATFORM_FAMILY},linux>,64,32>
        LINE_MASK_WIDTH=$<IF:$<STREQUAL:${PLATFORM_FAMILY},stm32h745i>,16,32>
)

target_include_directories(core-gpio-base PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)

target_link_libraries(core-gpio-base PUBLIC
        config-struct
        etl::etl
)
