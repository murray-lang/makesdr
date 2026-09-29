cmake_minimum_required(VERSION 4.1)

project(radios-iq-basic-rx)

set(RADIO_IQ_RX_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/radios/iq/BasicIqRxRadio.h
        ${CMAKE_CURRENT_LIST_DIR}/src/BasicIqRxRadio.cpp
)

add_library(radios-iq-basic-rx STATIC ${RADIO_IQ_RX_FILES})

target_include_directories(radios-iq-basic-rx PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(radios-iq-basic-rx PUBLIC
        radios-base
        core-common-iq-basic-rx
        settings-control-radio
        settings-model-radio-iq-basic-rx

)