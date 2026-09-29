cmake_minimum_required(VERSION 4.1)

project(core-common-iq-dual-rxtx)

set(DUAL_IQ_RXTX_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/iq/rxtx/DualIqRxTx.h
        ${CMAKE_CURRENT_LIST_DIR}/include/iq/rxtx/DualIqRxTx_Rx.h

        ${CMAKE_CURRENT_LIST_DIR}/src/DualIqRxTx.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/DualIqRxTx_Rx.cpp
)

add_library(core-common-iq-dual-rxtx STATIC ${DUAL_IQ_RXTX_FILES})

target_include_directories(core-common-iq-dual-rxtx PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(core-common-iq-dual-rxtx PUBLIC
        core-common-iq-base
        core-common-iq-common
        core-common-iq-io
        core-common-iq-pipeline-rx
        core-common-iq-pipeline-tx
        core-common-audio-mixer
        settings-model-radio-iq-dual-rxtx

)