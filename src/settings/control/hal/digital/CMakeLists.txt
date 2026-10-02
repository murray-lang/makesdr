cmake_minimum_required(VERSION 4.1)

project(settings-control-hal-digital)

set(GPIO_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/control/digital/DigitalInputTypes.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/control/digital/DigitalInput.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/control/digital/DigitalInputTypes.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/control/digital/DigitalInputsT.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/control/digital/DigitalInputFactory.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/control/digital/DigitalOutputT.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/control/digital/GpioBandSelectorT.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/control/digital/DigitalOutputsT.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/control/digital/DigitalOutputFactoryT.h

        ${CMAKE_CURRENT_LIST_DIR}/src/DigitalInput.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/DigitalInputFactory.cpp

)

set(GPIO_LIBRARIES
        config-struct
        core-gpio-base
        settings-control-base
        settings-control-source
        settings-control-sink
        settings-model-update-base
        settings-model-radio-base
        etl
)

if(TARGET_PLATFORM STREQUAL "stm32h745i-cm4")
    set(GPIO_LIBRARIES ${GPIO_LIBRARIES}
            core-freertos-gpio
#            core-stm32h745i-drivers-bsp-disco-cm4
    )
elseif(TARGET_PLATFORM STREQUAL "stm32h745i-cm7")
    set(GPIO_LIBRARIES ${GPIO_LIBRARIES}
            core-freertos-gpio
    )
elseif(PLATFORM_FAMILY STREQUAL "linux")
    set(GPIO_LIBRARIES ${GPIO_LIBRARIES}
            core-linux-gpio
    )
endif()

add_library(settings-control-hal-digital STATIC ${GPIO_SOURCES})

target_include_directories(settings-control-hal-digital PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
#        ${CMAKE_SOURCE_DIR}/src/core/thread/include
)

target_link_libraries(settings-control-hal-digital PUBLIC ${GPIO_LIBRARIES})
