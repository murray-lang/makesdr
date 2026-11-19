cmake_minimum_required(VERSION 4.1)

project(core-freertos-transport-radio)

set(COMMON_TRANSPORT_CLIENT_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/freertos/radio/IqCapableRadioTransportT.h
)

add_library(core-freertos-transport-radio INTERFACE)
target_sources(core-freertos-transport-radio INTERFACE ${COMMON_TRANSPORT_CLIENT_FILES})
target_include_directories(core-freertos-transport-radio SYSTEM INTERFACE
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)
target_link_libraries(core-freertos-transport-radio INTERFACE
        core-common-transport-base
        core-common-transport-meta
        core-common-transport-metering
        core-common-transport-settings
        core-common-transport-spectrum
        core-common-transport-updates
        core-freertos-transport-base
        core-freertos-thread
)
