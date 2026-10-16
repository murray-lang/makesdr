cmake_minimum_required(VERSION 4.1)

project(radios-iq-dual-rxtx-transport-qt-radio)

set(DUAL_IQ_RXTX_TRANSPORT_QT_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/radios/iq/QtDualIqRxTxMessageRings.h
        ${CMAKE_CURRENT_LIST_DIR}/include/radios/iq/DualIqRxTxTransport.h
        ${CMAKE_CURRENT_LIST_DIR}/src/DualIqRxTxTransport.cpp
)

add_library(radios-iq-dual-rxtx-transport-qt-radio STATIC ${DUAL_IQ_RXTX_TRANSPORT_QT_FILES})

target_include_directories(radios-iq-dual-rxtx-transport-qt-radio PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(radios-iq-dual-rxtx-transport-qt-radio PUBLIC
        core-common-transport-iq
        core-linux-transport-qt-iq
        radios-iq-dual-rxtx-settings-model
)