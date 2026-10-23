cmake_minimum_required(VERSION 4.1)

project(radios-iq-dual-rxtx-iq)

set(DUAL_IQ_RXTX_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/radios/iq/DualIqRxTxT.h
)

add_library(radios-iq-dual-rxtx-iq INTERFACE ${DUAL_IQ_RXTX_FILES})

target_include_directories(radios-iq-dual-rxtx-iq INTERFACE
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(radios-iq-dual-rxtx-iq INTERFACE
        core-common-iq-base
        core-common-iq-common
        core-common-iq-io
        core-common-iq-pipeline-rx
        core-common-iq-pipeline-tx
        core-common-audio-mixer
        radios-iq-dual-rx-iq
)