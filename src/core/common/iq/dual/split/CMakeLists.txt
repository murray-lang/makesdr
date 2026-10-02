cmake_minimum_required(VERSION 4.1)

project(core-common-iq-dual-split)

set(DUAL_IQ_SPLIT_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/iq/split/SplitBandDualIq.h
        ${CMAKE_CURRENT_LIST_DIR}/include/iq/split/SplitBandDualIq_Rx.h
#        ${CMAKE_CURRENT_LIST_DIR}/include/iq/split/DualIqRxTx_Rx.h
#
        ${CMAKE_CURRENT_LIST_DIR}/src/SplitBandDualIq.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/SplitBandDualIq_Rx.cpp
#        ${CMAKE_CURRENT_LIST_DIR}/src/DualIqRxTx_Rx.cpp
)

add_library(core-common-iq-dual-split STATIC ${DUAL_IQ_SPLIT_FILES})

target_include_directories(core-common-iq-dual-split PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(core-common-iq-dual-split PUBLIC
        core-common-iq-base
        core-common-iq-common
        core-common-iq-io
        core-common-iq-pipeline-rx
        core-common-iq-pipeline-tx
        core-common-audio-mixer
        settings-model-radio-iq-dual-split

)