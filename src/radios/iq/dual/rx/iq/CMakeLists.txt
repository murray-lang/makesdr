cmake_minimum_required(VERSION 4.1)

project(radios-iq-dual-rx-iq)

set(DUAL_IQ_RX_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/radios/iq/DualIqRxT.h
)

add_library(radios-iq-dual-rx-iq INTERFACE ${DUAL_IQ_RX_FILES})

target_include_directories(radios-iq-dual-rx-iq INTERFACE
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(radios-iq-dual-rx-iq INTERFACE
        core-common-iq-base
        core-common-iq-common
        core-common-iq-io
        core-common-iq-pipeline-rx
        core-common-iq-pipeline-tx
        core-common-audio-mixer
        config-struct

)