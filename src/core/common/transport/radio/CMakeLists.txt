cmake_minimum_required(VERSION 4.1)

project(core-common-transport-radio)

set(RADIO_TRANSPORT_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/radio/ClientTransportT.h
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/radio/RadioMessageExchangeT.h
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/radio/RadioTransportT.h
)

add_library(core-common-transport-radio INTERFACE ${RADIO_TRANSPORT_FILES})

target_include_directories(core-common-transport-radio INTERFACE
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(core-common-transport-radio INTERFACE
        core-common-transport-base
        settings-model-message
        settings-model-radios-iq-basic-common
        settings-model-data-band
        settings-model-data-mode
)