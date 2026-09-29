cmake_minimum_required(VERSION 4.1)

project(core-common-iq-basic-rx)

set(IQ_RX_BASIC_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/iq/rx/BasicIqRx.h
        ${CMAKE_CURRENT_LIST_DIR}/src/BasicIqRx.cpp
)

add_library(core-common-iq-basic-rx STATIC ${IQ_RX_BASIC_FILES})

target_include_directories(core-common-iq-basic-rx PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(core-common-iq-basic-rx PUBLIC
        core-common-iq-base
        core-common-iq-pipeline-rx
        core-common-iq-io
        settings-model-radio-iq-basic-rx

)