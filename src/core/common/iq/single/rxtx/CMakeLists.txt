cmake_minimum_required(VERSION 4.1)

project(radios-iq-single-rx-iqtx)

set(IQ_RXTX_BASIC_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/iq/rxtx/SingleIqRxTx.h
        ${CMAKE_CURRENT_LIST_DIR}/include/iq/rxtx/SingleIqRxTx_Rx.h

        ${CMAKE_CURRENT_LIST_DIR}/src/SingleIqRxTx.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/SingleIqRxTx_Rx.cpp

)

add_library(radios-iq-single-rx-iqtx STATIC ${IQ_RXTX_BASIC_FILES})

target_include_directories(radios-iq-single-rx-iqtx PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(radios-iq-single-rx-iqtx PUBLIC
        core-common-iq-base
        core-common-iq-common
        core-common-iq-io
        core-common-iq-pipeline-rx
        core-common-iq-pipeline-tx
        settings-model-radio-iq-single-rxtx

)