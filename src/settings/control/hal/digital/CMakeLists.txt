cmake_minimum_required(VERSION 4.1)

project(settings-control-hal-digital)

set(GPIO_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/control/digital/DigitalInputTypes.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/control/digital/DigitalInput.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/control/digital/DigitalInputTypes.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/control/digital/DigitalInputs.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/control/digital/DigitalInputFactory.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/control/digital/DigitalOutput.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/control/digital/GpioBandSelector.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/control/digital/DigitalOutputs.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/control/digital/DigitalOutputFactory.h

        ${CMAKE_CURRENT_LIST_DIR}/src/DigitalInput.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/DigitalInputs.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/DigitalInputFactory.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/DigitalOutput.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/GpioBandSelector.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/DigitalOutputs.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/DigitalOutputFactory.cpp

)

set(GPIO_LIBRARIES
        config-struct
        core-gpio-base
        settings-control-base
        settings-control-source
        settings-control-sink
        settings-model-core
        etl
)

if(TARGET_PLATFORM STREQUAL "stm32h745i-cm4")
    set(GPIO_LIBRARIES ${GPIO_LIBRARIES}
            core-freertos-gpio-cm4
            core-stm32h745i-thread-cm4
            core-stm32h745i-drivers-bsp-disco-cm4
    )
elseif(TARGET_PLATFORM STREQUAL "stm32h745i-cm7")
    set(GPIO_LIBRARIES ${GPIO_LIBRARIES}
            core-freertos-gpio-cm7
            core-stm32h745i-thread-cm7
    )
endif()

add_library(settings-control-hal-digital STATIC ${GPIO_SOURCES})

target_include_directories(settings-control-hal-digital PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
#        ${CMAKE_SOURCE_DIR}/src/core/thread/include
)

target_link_libraries(settings-control-hal-digital PUBLIC ${GPIO_LIBRARIES})
