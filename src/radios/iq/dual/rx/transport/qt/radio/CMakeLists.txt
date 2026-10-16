cmake_minimum_required(VERSION 4.1)

project(radios-iq-dual-rx-transport-qt-radio)

set(DUAL_IQ_RX_TRANSPORT_QT_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/radios/iq/QtDualIqRxMessageRings.h
        ${CMAKE_CURRENT_LIST_DIR}/include/radios/iq/DualIqRxTransport.h
        ${CMAKE_CURRENT_LIST_DIR}/src/DualIqRxTransport.cpp
)

add_library(radios-iq-dual-rx-transport-qt-radio STATIC ${DUAL_IQ_RX_TRANSPORT_QT_FILES})

target_include_directories(radios-iq-dual-rx-transport-qt-radio PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(radios-iq-dual-rx-transport-qt-radio PUBLIC
        core-common-transport-iq
        core-linux-transport-qt-iq
        radios-iq-dual-rx-settings-model
)