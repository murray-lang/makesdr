cmake_minimum_required(VERSION 4.1)

project(radios-iq-dual-rxtx)

set(DUAL_SPLIT_RADIO_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/radios/iq/DualIqRxTxRadio.h
        ${CMAKE_CURRENT_LIST_DIR}/src/DualIqRxTxRadio.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/RadioControl.cpp
)

add_library(radios-iq-dual-rxtx STATIC ${DUAL_SPLIT_RADIO_FILES})

target_include_directories(radios-iq-dual-rxtx PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

set(DUAL_SPLIT_RADIO_LIBS
        radios-base
        radios-iq-dual-rxtx-iq
        settings-control-radio
        settings-model-radio-iq-dual-rxtx
        radios-iq-dual-rxtx-iq
)

if(IS_QT)
    list(APPEND DUAL_SPLIT_RADIO_LIBS
            radios-iq-dual-rxtx-transport-qt-globals
            core-linux-transport-qt-radio-iq
            core-linux-transport-qt-settings
            core-linux-transport-qt-globals-iq
            core-linux-transport-qt-globals-meta
    )
endif()

if(IS_LINUX)
    list(APPEND DUAL_SPLIT_RADIO_LIBS
            core-linux-thread
    )
else()
    list(APPEND DUAL_SPLIT_RADIO_LIBS
            core-freertos-thread
    )
endif()

target_link_libraries(radios-iq-dual-rxtx PUBLIC
        ${DUAL_SPLIT_RADIO_LIBS}
)
