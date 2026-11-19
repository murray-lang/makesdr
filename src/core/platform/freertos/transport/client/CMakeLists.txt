cmake_minimum_required(VERSION 4.1)

project(core-freertos-transport-client)

set(COMMON_TRANSPORT_CLIENT_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/freertos/client/IqCapableClientTransportT.h
)

add_library(core-freertos-transport-client INTERFACE)
target_sources(core-freertos-transport-client INTERFACE ${COMMON_TRANSPORT_CLIENT_FILES})
target_include_directories(core-freertos-transport-client SYSTEM INTERFACE
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)
target_link_libraries(core-freertos-transport-client INTERFACE
        core-common-transport-base
        core-common-transport-meta
        core-common-transport-metering
        core-common-transport-settings
        core-common-transport-spectrum
        core-common-transport-updates
        core-freertos-transport-base
        core-freertos-thread
)
