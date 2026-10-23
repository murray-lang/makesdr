cmake_minimum_required(VERSION 4.1)

project(radios-iq-single-rx-iq)

set(IQ_RX_BASIC_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/radios/iq/SingleIqRxT.h
)

add_library(radios-iq-single-rx-iq INTERFACE ${IQ_RX_BASIC_FILES})

target_include_directories(radios-iq-single-rx-iq INTERFACE
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(radios-iq-single-rx-iq INTERFACE
        core-common-iq-base
        core-common-iq-common
        core-common-iq-io
        core-common-iq-pipeline-rx
        core-common-iq-pipeline-tx
        core-common-audio-mixer
        config-struct

)