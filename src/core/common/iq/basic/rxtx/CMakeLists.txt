cmake_minimum_required(VERSION 4.1)

project(core-common-iq-basic-rxtx)

set(IQ_RXTX_BASIC_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/iq/rxtx/BasicIqRxTx.h
        ${CMAKE_CURRENT_LIST_DIR}/include/iq/rxtx/BasicIqRxTx_Rx.h

        ${CMAKE_CURRENT_LIST_DIR}/src/BasicIqRxTx.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/BasicIqRxTx_Rx.cpp

)

add_library(core-common-iq-basic-rxtx STATIC ${IQ_RXTX_BASIC_FILES})

target_include_directories(core-common-iq-basic-rxtx PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(core-common-iq-basic-rxtx PUBLIC
        core-common-iq-base
        core-common-iq-common
        core-common-iq-io
        core-common-iq-pipeline-rx
        core-common-iq-pipeline-tx
        settings-model-radio-iq-basic-rxtx

)