cmake_minimum_required(VERSION 3.25)

project(makesdr-settings-control-hal-gpio)

set(GPIO_SOURCES
        include/settings/control/gpio/base/Gpio.h
        include/settings/control/gpio/base/GpioLines.h
        include/settings/control/gpio/base/GpioInputLines.h
        include/settings/control/gpio/base/GpioEvent.h
        include/settings/control/gpio/base/GpioEventQueue.h
        include/settings/control/gpio/base/GpioLinesRequestBase.h
        include/settings/control/gpio/base/GpioInputLinesRequest.h
        include/settings/control/gpio/base/GpioOutputLinesRequest.h
        include/settings/control/gpio/DigitalInputLinesRequest.h
        include/settings/control/gpio/DigitalOutputLinesRequest.h
        include/settings/control/gpio/DigitalInputTypes.h
        include/settings/control/gpio/DigitalInput.h
        include/settings/control/gpio/RotaryEncoder.h
        include/settings/control/gpio/DigitalInputs.h
        include/settings/control/gpio/DigitalInputFactory.h
        include/settings/control/gpio/DigitalOutput.h
        include/settings/control/gpio/GpioBandSelector.h
        include/settings/control/gpio/DigitalOutputs.h
        include/settings/control/gpio/DigitalOutputFactory.h

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

add_library(makesdr-settings-control-hal-gpio STATIC ${GPIO_SOURCES} ${GPIO_LINUX_SOURCES})

target_include_directories(makesdr-settings-control-hal-gpio PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
#        ${CMAKE_SOURCE_DIR}/src/core/thread/include
)

target_link_libraries(makesdr-settings-control-hal-gpio PUBLIC
        makesdr-config-struct
        makesdr-core-thread
        makesdr-settings-control-base
        makesdr-settings-control-source
        makesdr-settings-control-sink
        makesdr-settings-model-core
        etl
)
