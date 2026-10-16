cmake_minimum_required(VERSION 4.1)

project(radios-iq-dual-split-transport-qt-radio)

set(DUAL_IQ_SPLIT_TRANSPORT_QT_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/radios/iq/QtSplitBandDualIqMessageRings.h
        ${CMAKE_CURRENT_LIST_DIR}/include/radios/iq/SplitBandDualIqTransport.h
        ${CMAKE_CURRENT_LIST_DIR}/src/SplitBandDualIqTransport.cpp
)

add_library(radios-iq-dual-split-transport-qt-radio STATIC ${DUAL_IQ_SPLIT_TRANSPORT_QT_FILES})

target_include_directories(radios-iq-dual-split-transport-qt-radio PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(radios-iq-dual-split-transport-qt-radio PUBLIC
        core-common-transport-iq
        core-linux-transport-qt-settings
        core-linux-transport-qt-iq
        radios-iq-dual-split-settings-model
)