cmake_minimum_required(VERSION 4.1)

project(RadioSettingsProto-settings-control-hal-gpio)

set(GPIO_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/include/gpio/base/Gpio.h
        ${CMAKE_CURRENT_LIST_DIR}/include/gpio/base/GpioLines.h
        ${CMAKE_CURRENT_LIST_DIR}/include/gpio/base/GpioInputLines.h
        ${CMAKE_CURRENT_LIST_DIR}/include/gpio/base/GpioEvent.h
        ${CMAKE_CURRENT_LIST_DIR}/include/gpio/base/GpioEventQueue.h
        ${CMAKE_CURRENT_LIST_DIR}/include/gpio/base/GpioLinesRequestBase.h
        ${CMAKE_CURRENT_LIST_DIR}/include/gpio/base/GpioInputLinesRequest.h
        ${CMAKE_CURRENT_LIST_DIR}/include/gpio/base/GpioOutputLinesRequest.h
        ${CMAKE_CURRENT_LIST_DIR}/include/gpio/DigitalInputLinesRequest.h
        ${CMAKE_CURRENT_LIST_DIR}/include/gpio/DigitalOutputLinesRequest.h
        ${CMAKE_CURRENT_LIST_DIR}/include/gpio/DigitalInputTypes.h
        ${CMAKE_CURRENT_LIST_DIR}/include/gpio/DigitalInput.h
        ${CMAKE_CURRENT_LIST_DIR}/include/gpio/RotaryEncoder.h
        ${CMAKE_CURRENT_LIST_DIR}/include/gpio/DigitalInputs.h
        ${CMAKE_CURRENT_LIST_DIR}/include/gpio/DigitalInputFactory.h
        ${CMAKE_CURRENT_LIST_DIR}/include/gpio/DigitalOutput.h
        ${CMAKE_CURRENT_LIST_DIR}/include/gpio/GpioBandSelector.h
        ${CMAKE_CURRENT_LIST_DIR}/include/gpio/DigitalOutputs.h
        ${CMAKE_CURRENT_LIST_DIR}/include/gpio/DigitalOutputFactory.h

        ${CMAKE_CURRENT_LIST_DIR}/src/GpioLines.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/DigitalInput.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/RotaryEncoder.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/DigitalInputs.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/DigitalInputFactory.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/DigitalOutput.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/GpioBandSelector.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/DigitalOutputs.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/DigitalOutputFactory.cpp

)

set(GPIO_LINUX_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/src/platform/linux/Gpiod.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/platform/linux/GpioEventQueueGpiod.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/platform/linux/DigitalInputLinesRequestGpiod.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/platform/linux/DigitalOutputLinesRequestGpiod.cpp
)

add_library(RadioSettingsProto-settings-control-hal-gpio STATIC ${GPIO_SOURCES} ${GPIO_LINUX_SOURCES})

target_include_directories(RadioSettingsProto-settings-control-hal-gpio PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
#        ${CMAKE_SOURCE_DIR}/src/core/thread/include
)

target_link_libraries(RadioSettingsProto-settings-control-hal-gpio PUBLIC
        RadioSettingsProto-config
        RadioSettingsProto-core-thread
        RadioSettingsProto-settings-control-base
        RadioSettingsProto-settings-control-source
        RadioSettingsProto-settings-control-sink
        RadioSettingsProto-settings-model-mutable
        etl
)
