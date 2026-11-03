cmake_minimum_required(VERSION 4.1)

project(core-stm32h745i-transport)

set(STM32_TRANSPORT_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/stm32h745i/globalStm32H745Rings.h
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/stm32h745i/HsemNotifier.h
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/stm32h745i/Stm32H745IqCapableRadioTransportT.h
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/stm32h745i/Stm32H745MessageRings.h

        ${CMAKE_CURRENT_LIST_DIR}/src/globalStm32H745Rings.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/HsemNotifier.cpp
)

add_library(core-stm32h745i-transport STATIC ${STM32_TRANSPORT_FILES})

target_include_directories(core-stm32h745i-transport PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

if(TARGET_PLATFORM STREQUAL "stm32h745i-cm4")
    target_link_libraries(core-stm32h745i-transport PUBLIC
            core-common-transport-base
            core-common-transport-meta
            core-common-transport-metering
            core-common-transport-radio
            core-common-transport-settings
            core-common-transport-spectrum
            core-common-transport-updates
            core-freertos-transport-base
            core-stm32h745i-drivers-hal-cm4
    )
elseif(TARGET_PLATFORM STREQUAL "stm32h745i-cm7")
    target_link_libraries(core-stm32h745i-transport PUBLIC
            core-common-transport-base
            core-common-transport-meta
            core-common-transport-metering
            core-common-transport-radio
            core-common-transport-settings
            core-common-transport-spectrum
            core-common-transport-updates
            core-freertos-transport-base
            core-stm32h745i-drivers-hal-cm7
    )
endif()
